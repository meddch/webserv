/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:05:29 by azari             #+#    #+#             */
/*   Updated: 2023/11/16 15:18:40 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

Response::Response():

	_version(""),
	_status(""),
	_body(""),
	_response("")
{}

std::string Response::generateResponse(Request& request){
	
	_version = request._headers["httpVersion"];
	// if (_version.compare("HTTP/1.1"))
	// 	// raise error 505 HTTP Version Not Supported
	// 	// return;


	// _status = generateStatusPhrase(200);
	_status = generateStatusPhrase(request.getStatusCode());
	_response.append(_version + _status + "\r\n");
	// find pathfile to put in mime type
	
	
	
	_headers["Content-Type"] = findMimeType(".html");
	_headers["Content-Length"] = "867"; // to Implement
	_headers["Server"] = "Webserv/1.0.0 (mechane-azari)";
	_headers["Date"] = generateResponseDate();
	_headers["Connection"] = "keep-alive"; // to Implement
	for(std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
		_response.append(it->first + ": " + it->second + "\r\n");
	_response.append("\r\n");
	std::string bodyfile = "/Users/azari/Desktop/webserv/samples/index.html";
	std::ifstream file(bodyfile.c_str());
	if (file.fail())
		throw std::runtime_error("Failed to open file: " + bodyfile);

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.empty())
		throw std::runtime_error("Empty file: " + bodyfile);
	_body = content;
	std::cout << "[\n" << _response << "]" << std::endl;
	_response.append(_body);

	return _response;
}

