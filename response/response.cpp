/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:05:29 by azari             #+#    #+#             */
/*   Updated: 2023/11/17 16:56:38 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response():
	_statusCode(0),
	_version(""),
	_status(""),
	_body(""),
	_KEEPALIVE(false),
	_response("")
{}

void Response::initResponseHeaders(){

	_response.append(_version + _status + "\r\n");
	_headers["Content-Type"] = findMimeType(".html");
	_headers["Content-Length"] = _contentLength;
	if (_contentLength.empty())
		_headers["Content-Length"] = "761";
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	_headers["Date"] = generateResponseDate();
	_headers["Connection"] = isConnectionKeepAlive() ? "keep-alive" : "close"; // to Implement
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		_response.append(it->first + ": " + it->second + "\r\n");
	_response.append("\r\n");
}

bool Response::handleResponseError(Request& request){
	
	_version = request._headers["httpVersion"];
	if (_version.compare("HTTP/1.1"))
		if (!_statusCode)
			_statusCode = 505; // HTTP Version Not Supported
	// _statusCode = 502;
	_status = generateStatusPhrase(_statusCode);
	if (_statusCode && _statusCode != 200){

		std::string errorBody = "<html><head><title>" + _status + "</title></head><body><center><h1>" + _status + "</h1></center><hr><center>Webserv/1.0.0 (mechane-azari)</center></body></html>";
		_contentLength = std::to_string(errorBody.length());
		initResponseHeaders();
		_response.append(errorBody);
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
	
	_statusCode = request.getStatusCode();
	if (handleResponseError(request)){

		return _response;
	}
	initResponseHeaders();
	std::string bodyfile = "/Users/azari/Desktop/webserv/samples/index.html";
	std::ifstream file(bodyfile.c_str());
	if (file.fail())
		throw std::runtime_error("Failed to open file: " + bodyfile);

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.empty())
		throw std::runtime_error("Empty file: " + bodyfile);
	_body = content;
	_response.append(_body);

	std::cout << "[\n" << _response << "]" << std::endl;
	return _response;
}

