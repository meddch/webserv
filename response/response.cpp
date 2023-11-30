/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:05:29 by azari             #+#    #+#             */
/*   Updated: 2023/11/30 10:09:17 by mechane          ###   ########.fr       */
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
	_headerSent(false)
{}

void Response::initResponseHeaders(){

	response ="HTTP/1.1" + _status + "\r\n";
	_headers["Content-Type"] = findMimeType(filePath.substr(filePath.find_last_of(".")));
	if (_contentLength.empty())
		_headers["Content-Length"] = _contentLength;
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	_headers["Connection"] = isConnectionKeepAlive() ? "keep-alive" : "close"; 
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		response.append(it->first + ": " + it->second + "\r\n");
	response.append("\r\n");
}

bool Response::handleResponseError(Request& request){
	
	_version = request._headers["httpVersion"];
	if (_version.compare("HTTP/1.1"))
		setStatusCode(505); // HTTP Version Not Supported code :

	_status = generateStatusPhrase(_statusCode);
	if (_statusCode && _statusCode != 200){

		std::string errorBody = "<html><head><title>" + _status + "</title></head><body><center><h1>" + _status + "</h1></center><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
		_contentLength = std::to_string(errorBody.length());
		initResponseHeaders();
		response.append(errorBody);
		return true;
	}
	return false;
}

bool Response::isConnectionKeepAlive(){
	if (_KEEPALIVE)
		return true;
	return false;
}


std::string Response::generateResponse(Request& request){

	if (_statusCode){
	
		handleResponseError(request);
		return response;
	}
	std::string uri = request._headers["URI"];
	_statusCode = request.getStatusCode();
	_status = generateStatusPhrase(_statusCode);

	filePath = "/Users/azari/Desktop/webserv/samples/index.html";
	if (uri == "/John_wick.mp4" && uri != "/favicon.ico")
		filePath = "/Users/azari/goinfre/John_wick.mp4";
	else if (uri != "/" && uri != "/favicon.ico")
		filePath = "/Users/azari/Desktop/webserv/samples" + uri;
	
	initResponseHeaders();
	
	if (handleResponseError(request))
		return response;

	return response;
}

std::string generate_auto_index(std::string path)
{
	std::string auto_index = "<html><head><title>Index of " + path + "</title></head><body><h1>Index of " + path + "</h1><hr><pre>";
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) 
	{
		while ((ent = readdir (dir)) != NULL)
		{
			auto_index += "<a href=\"" + path + "/" + ent->d_name + "\">" + ent->d_name + "</a><br>";
		}
		closedir (dir);
	}
	else 
	{
		perror ("");
		return "";
	}
	auto_index += "</pre><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
	return auto_index;

}
