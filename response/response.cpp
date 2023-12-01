/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:05:29 by azari             #+#    #+#             */
/*   Updated: 2023/12/01 18:38:44 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

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
	readyToSend(false)
{}

void Response::initResponseHeaders(Request& request){

	_version = request._headers["httpVersion"];
	if (_statusCode == 0)
		_statusCode = request.getStatusCode();
	_status = generateStatusPhrase(_statusCode);
	std::cout << "STATUS: " << _status << std::endl;
	response = _version + _status + "\r\n";
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	if (request._headers["Method"] == "GET")
		_headers["Content-Length"] = _contentLength;
	if (_headers.find("Content-Type") == _headers.end() && _statusCode != 204 && _statusCode != 304 && request._headers["Method"] == "GET")
	{
		_headers["Content-Type"] = findMimeType(filePath.substr(filePath.find_last_of(".")));
		_headers["Accept-Ranges"] = "bytes";
	}
	
	puts("initResponseHeaders");
	// _headers["Connection"] = isConnectionKeepAlive() ? "keep-alive" : "close";
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

bool Response::isConnectionKeepAlive(){
	if (_KEEPALIVE)
		return true;
	return false;
}

void Response::generateAutoIndex(Request& request)
{
    std::string path = filePath;
    std::cout << "PATH: " << path << std::endl;
    std::string auto_index = "<html><head><title>Index of " + path + "</title></head>";
    auto_index += "<style>"
                  "body {font-family: Arial, sans-serif; background-color: #f0f0f0; color: #333;}"
                  "h1 {color: #005a9c;}"
                  "a {color: #005a9c; text-decoration: none;}"
                  "a:hover {text-decoration: underline;}"
                  "a.file {background: url(file_icon.png) no-repeat left center; padding-left: 20px;}"
                  "a.directory {background: url(directory_icon.png) no-repeat left center; padding-left: 20px;}"
                  "</style></head><body><h1> 42-webserv</h1><hr><pre>";
    DIR *dir;
    struct dirent *ent;
    struct stat path_stat;
    if ((dir = opendir (path.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string full_path = path + "/" + ent->d_name;
            stat(full_path.c_str(), &path_stat);
            auto_index += "<a href=\"" + ((std::string)ent->d_name) + (S_ISDIR(path_stat.st_mode) ? "/" : "") + "\" class=\"" + (S_ISDIR(path_stat.st_mode) ? "directory" : "file") + "\">" + ent->d_name + "</a><br><br>";
        }
        closedir (dir);
    }
    std::cout << "AUTO INDEX: " << auto_index << std::endl;
    auto_index += "</pre><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
    _contentLength = std::to_string(auto_index.length());
    _headers["Content-Type"] = "text/html";
    initResponseHeaders(request);
    response += auto_index;
    readyToSend = true;
}