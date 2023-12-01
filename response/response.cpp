/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:05:29 by azari             #+#    #+#             */
/*   Updated: 2023/12/01 18:21:16 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include <sys/_types/_ssize_t.h>

Response::Response():
	_statusCode(0),
	_version(""),
	_status(""),
	_body(""),
	_KEEPALIVE(false),
	_isfileRead(false),
	response(""),
	// _contentLength(""),
	_headerSent(false),
	readyToSend(false),
	fd(0),
	root(""),
	_fileSize(0),
	_offset(0)
{}

void Response::initResponseHeaders(Request& request){

	_version = request._headers["httpVersion"];
	if (_statusCode == 0)
		_statusCode = request.getStatusCode();
	_status = generateStatusPhrase(_statusCode);
	response = _version + _status + "\r\n";
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	_headers["Content-Length"] = _contentLength;
	if (_headers.find("Content-Type") == _headers.end())
		_headers["Content-Type"] = findMimeType(filePath.substr(filePath.find_last_of(".")));
	_headers["Accept-Ranges"] = "bytes";
	_headers["Connection"] = "keep-alive" ;
	_headers["cache-control"] = "max-age=3600 public";
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		response.append(it->first + ": " + it->second + "\r\n");
	response.append("\r\n");
}

bool Response::handleResponseError(Request& request, std::string code){
	
	_version = request._headers["httpVersion"];
	_status = generateStatusPhrase(std::stoi(code));
	response = _version + _status + "\r\n";
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	std::string errorBody = "<html><head><title>" + _status + "</title></head><body><center><h1>" + _status + "</h1></center><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
	_contentLength = std::to_string(errorBody.length());
	_headers["Content-Length"] = _contentLength;
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		response.append(it->first + ": " + it->second + "\r\n");
	response.append("\r\n");
	response.append(errorBody);
	readyToSend = true;
	return true;

}
void Response::generateChunkedResponse(){
	
	response ="HTTP/1.1 206 Partial Content\r\n";
	_headers["Content-Length"] = std::to_string(std::min((size_t)BYTES, size_t(_fileSize - _offset)));
	_headers["Content-Range"] = "bytes " + std::to_string(_offset) + "-" + std::to_string(std::min((ssize_t)(_offset + BYTES), (ssize_t)(_fileSize))) + "/" + std::to_string(_fileSize);
	_headers["Accept-Ranges"] = "bytes";
	_headers["Content-Type"] = findMimeType(filePath.substr(filePath.find_last_of(".")));
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	_headers["Connection"] =  "close";
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		response.append(it->first + ": " + it->second + "\r\n");
	response.append("\r\n");
	
	

}

bool Response::isConnectionKeepAlive(){
	if (_KEEPALIVE)
		return true;
	return false;
}




void Response::generateAutoIndex(Request& request)
{
	std::string path = filePath;
    std::string auto_index = "<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><hr><pre>";
    DIR *dir;
    struct dirent *ent;
    struct stat path_stat;
    if ((dir = opendir (path.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL)
        {
            if (ent->d_name[0] == '.')
				continue;
			std::string full_path = path + "/" + ent->d_name;
            stat(full_path.c_str(), &path_stat);
            auto_index += "<a href=\"" + ((std::string)ent->d_name) + (S_ISDIR(path_stat.st_mode) ? "/" : "") + "\">" + ent->d_name + "</a><br><br>";
        }
        closedir (dir);
    }
    auto_index += "</pre><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
    _contentLength = std::to_string(auto_index.length());
    _headers["Content-Type"] = "text/html";
    initResponseHeaders(request);
    response += auto_index;
    readyToSend = true;
}


