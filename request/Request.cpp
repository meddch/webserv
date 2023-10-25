/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:21:09 by azari             #+#    #+#             */
/*   Updated: 2023/10/25 10:29:01 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::~Request(){
	_headers.clear();
}

Request::Request():  _errorCode(0), _bodyRead(false), _REQ(""), _lastHeaderPos(0) , _parsePos(0), _contentLength(-1){
	_Boundaries.clear();
}


BoundRequest::BoundRequest(std::string boundary, std::string _REQ):  _boundary(boundary), _boundaryEndPos(0), _boundaryNumber(0){
	_headers.clear();
	this->_REQ = _REQ;
}

void	Request::toString(){


	// std::cout << "\n\%\%\%\%\%\%\%\% Request \%\%\%\%\%\%\%\%\%%\n" << std::endl;
	// std::cout << _REQ << std::endl;
		
	puts("\n\%\%\%\%\%\%\%\% Headers \%\%\%\%\%\%\%\%\%%\n");
    for(std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
        std::cout << "[" << it->first << "]--[" << it->second << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Body \%\%\%\%\%\%\%\%\%%\n");
	std::cout << "[" << _body << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Boundaries \%\%\%\%\%\%\%\%\%%\n");
	for (std::vector<BoundRequest>::iterator it = _Boundaries.begin(); it != _Boundaries.end(); ++it){
		for (std::map<std::string, std::string>::const_iterator it2 = it->_headers.begin(); it2 != it->_headers.end(); ++it2){
			std::cout << "[" << it2->first << "]--[" << it2->second << "]" << std::endl;
			std::cout << "[" << it->_body << "]" << std::endl;	
		}
		puts("________________________________________________________________");
	}
}

void Request::parseRequestLine(std::string requestLine){
    
    std::stringstream line(requestLine);
    std::string methods[3];
    int i;
    methods[0] = "GET";    
    methods[1] = "POST";    
    methods[2] = "DELETE";    

    line >> _headers["Method"] >> _headers["URI"] >> _headers["httpVersion"];
	_requestMethod = _headers["Method"];
    for (i = 0; i < 3; i++)
        if (methods[i] == _headers["Method"])
            break;
    (i >= 3) && (_errorCode = 501); // 501: Method Not Implemented
    if (_headers["URI"].find("?") != std::string::npos){
        std::string newURI = _headers["URI"].substr(0, _headers["URI"].find("?"));
        _headers["Queries"] = _headers["URI"].substr(_headers["URI"].find("?") + 1, _headers["URI"].npos);
        _headers["URI"] = newURI;
    }
	_parsePos = _REQ.find("\r\n") + 2;
	_lastHeaderPos = _REQ.find("\r\n\r\n");
}

void	Request::parseRequestHeaders(){

	std::string headerKey, headerValue;
	while (_parsePos < _lastHeaderPos){

		headerKey =_REQ.substr(_parsePos,_REQ.find(":", _parsePos) - _parsePos);
		headerValue =_REQ.substr(_REQ.find(":", _parsePos) + 2,_REQ.find("\r\n", _parsePos) -_REQ.find(":", _parsePos) - 2);
		(headerKey == "content-length") && (_contentLength = std::stoi(headerValue));
		this->_headers[headerKey] = headerValue;
	    _parsePos = _REQ.find("\r\n", _parsePos) + 2;
	}
}


void    Request::parseRequestBody()
{
    std::string key, value;
	if (_headers.find("content-length") != _headers.end() && _headers.find("Transfer-Encoding") == _headers.end() && _headers["content-type"].find("multipart/form-data") == std::string::npos)
    	_body =_REQ.substr(_parsePos, std::stoi(_headers["content-length"]));
    else if (_headers["content-type"].find("multipart/form-data") != std::string::npos){
        
        BoundRequest obj("--" + _headers["content-type"].substr(_headers["content-type"].find("boundary=") + 9, _headers["content-type"].npos), _REQ);
		obj._boundaryEndPos = _REQ.find(obj._boundary + "--");
		size_t pos = _REQ.find(obj._boundary);
		while (pos < obj._boundaryEndPos){
			obj._boundaryNumber++;
			pos = _REQ.find(obj._boundary, pos + obj._boundary.length() + 2);
		}
		_parsePos = 0;
		while (obj._boundaryNumber--){

			obj._parsePos = _REQ.find(obj._boundary, _parsePos) + obj._boundary.length() + 2;
			obj._lastHeaderPos = _REQ.find("\r\n\r\n", obj._parsePos);
			obj.parseRequestHeaders();
			obj._parsePos = _REQ.find(obj._boundary, _parsePos) + obj._boundary.length() + 2;
			obj._BoundaryNext = _REQ.find(obj._boundary, obj._parsePos);
			obj._parsePos = _REQ.find("\r\n\r\n", obj._parsePos) + 4;
			obj._body = _REQ.substr(obj._parsePos, obj._BoundaryNext - obj._parsePos - 2);
			_Boundaries.push_back(obj);
			_parsePos = obj._BoundaryNext;
		}
	}
    else if (_headers.find("Transfer-Incoding") != _headers.end() && _headers["Transfer-Encoding"].find("chunked") != std::string::npos){

		std::string chunk_size;
		std::string chunk_data;
		size_t pos = 0;
		size_t pos_end =_REQ.find("0\r\n\r\n");

		while (pos < pos_end){

			chunk_size =_REQ.substr(pos,_REQ.find("\r\n", pos) - pos);
			chunk_data =_REQ.substr(_REQ.find("\r\n", pos) + 2, std::stoi(chunk_size, 0, 16));
			pos =_REQ.find("\r\n", pos) + 2 + std::stoi(chunk_size, 0, 16);
			_body += chunk_data;
		}

		std::cout << "[" <<_body << "]" << std::endl;
	}
    else
        _errorCode = 415; // 415: Unsupported Media Type;

	// find how to treat CGI
}


size_t Request::getErrorCode() const{
	return _errorCode;
}

std::string Request::getRequestMethod() const{
	return _requestMethod;
}

std::string Request::getRequestString() const{
	return _REQ;
}

ssize_t Request::getContentLength() const{
	return _contentLength;
}

size_t Request::getLastHeaderPos() const{
	return _lastHeaderPos;
}

size_t Request::getBodyPosition() const{
	return _lastHeaderPos + 4;
}

void Request::setRequestMethod(std::string Method){
	_requestMethod = Method;
}

void Request::setRequestString(std::string requestString){
	_REQ = requestString;
	_parsePos = 0;
}

void Request::setContentLength(size_t contentLength){
	_contentLength = contentLength;
}

void Request::setLastHeaderPos(size_t lastHeaderPos){
	_lastHeaderPos = lastHeaderPos;
}

void Request::setErrorCode(size_t errorCode){
	_errorCode = errorCode;
}

bool Request::getBodyRead() const
{
	return _bodyRead;
}

void Request::setBodyRead(bool bodyRead)
{
	_bodyRead = bodyRead;
}