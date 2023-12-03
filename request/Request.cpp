/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:21:09 by azari             #+#    #+#             */
/*   Updated: 2023/12/03 18:02:09 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::~Request(){
	_headers.clear();
}

Request::Request():

	_StatusCode(0),
	_bodyRead(false),
	_hostExist(false),
	_transferEncodingExist(false),
	_chunked(false),
	_contentLengthExist(false),
	_reqParsed(false),
	uri(""),
	_REQ(""),
	_lastHeaderPos(0),
	_parsePos(0),
	_contentLength(-1)
{
	_Boundaries.clear();
}

BoundRequest::BoundRequest(std::string boundary, std::string _REQ):  _boundary(boundary), _boundaryEndPos(0), _boundaryNumber(0){
	_headers.clear();
	this->_REQ = _REQ;
	this->_reqParsed = true;
}

bool hasInvalidCharacter(const std::string& str)
{
    std::string set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";

    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
	{
        if (set.find(*it) == std::string::npos)
            return true;
    }
    return false;
}

std::string Request::parseURI(std::string uri){
	
	std::ostringstream decoded_uri;

	if (hasInvalidCharacter(uri))
		throw std::runtime_error("400");
	for (std::size_t i = 0; i < uri.length(); i++)
	{
		
		if (uri[i] == '%' && i + 2 < uri.length() && isxdigit(uri[i + 1]) && isxdigit(uri[i + 2]))
		{
			decoded_uri << static_cast<char>(std::strtol(uri.substr(i + 1, 2).c_str(), 0, 16));
			i += 2;
		}
		else if (uri[i] == '+')
			decoded_uri << ' ';
		else
			decoded_uri << uri[i];
		
	}
	std::cout << "decoded_uri: " << decoded_uri.str() << std::endl;
	std::cout << "uri: " << uri << std::endl;
	uri = decoded_uri.str();
	if (uri.length() == 0 || uri.length() > 2048)
		throw std::runtime_error("414");
    if (uri.find("?") != std::string::npos){
        std::string newURI = uri.substr(0, uri.find("?"));
        _headers["Queries"] = uri.substr(uri.find("?") + 1, uri.npos);
        uri = newURI;
    }

	return uri;
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

	std::cout << requestLine << std::endl;
    for (i = 0; i < 3; i++)
        if (methods[i] == _headers["Method"])
            break;
    if (i >= 3)
		throw std::runtime_error("501");
	if (_headers["httpVersion"] != "HTTP/1.1")
		throw std::runtime_error("505");

	_headers["URI"] = parseURI(_headers["URI"]);
	uri = _headers["URI"];
	std::cout << "URI: " << uri << std::endl;

	_parsePos = _REQ.find("\r\n") + 2;
	_lastHeaderPos = _REQ.find("\r\n\r\n");
}

std::string stringToLowercase(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

void Request::markExistance(const std::string& headerKey){

	if (headerKey == "content-length" || headerKey == "Content-Length")
		_contentLengthExist = true;
	if (headerKey == "transfer-encoding" || headerKey == "Transfer-Encoding")
		_transferEncodingExist = true;
	if (headerKey == "Host" || headerKey == "host")
		_hostExist = true;
	if (_transferEncodingExist == true && _headers["transfer-encoding"].find("chunked") != std::string::npos)
		_chunked = true;
	else if (_transferEncodingExist == true && _headers["Transfer-Encoding"].find("chunked") != std::string::npos)
		_chunked = true;
}

void Request::checkExistance(void){
	
	if (_transferEncodingExist == false && _contentLengthExist == false && _requestMethod == "POST")
		throw std::runtime_error("400");
	if (_hostExist == false)
	{
		throw std::runtime_error("400");
	}

	if (_transferEncodingExist == true && _chunked == false)
		throw std::runtime_error("501");
}

void	Request::parseRequestHeaders(){

	std::string headerKey, headerValue;
	while (_parsePos < _lastHeaderPos){

		headerKey =_REQ.substr(_parsePos,_REQ.find(":", _parsePos) - _parsePos);
		headerValue =_REQ.substr(_REQ.find(":", _parsePos) + 2,_REQ.find("\r\n", _parsePos) -_REQ.find(":", _parsePos) - 2);
		if (headerKey == "content-length" || headerKey == "Content-Length")
			_contentLength = std::atoi(headerValue.c_str());
		this->_headers[stringToLowercase(headerKey)] = headerValue;
	    _parsePos = _REQ.find("\r\n", _parsePos) + 2;
		markExistance(headerKey);
	}
	if (_reqParsed == false)
		checkExistance();
}


void    Request::parseRequestBody()
{
    std::string key, value;
	if (_headers.find("content-length") != _headers.end() && _headers.find("transfer-encoding") == _headers.end() && _headers["content-type"].find("multipart/form-data") == std::string::npos)
    	_body =_REQ.substr(_parsePos, std::atoi((_headers["content-length"]).c_str()));
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
    else if (_headers.find("transfer-encoding") != _headers.end() && _headers["transfer-encoding"].find("chunked") != std::string::npos){

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
	}
}

void Request::isMethodAllowed(std::vector<std::string> methods){
	
	for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
	{
		if (*it == _requestMethod)
			return ;
	}
	throw std::runtime_error("405");
}


size_t Request::getStatusCode() const{
	return _StatusCode;
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

void Request::setStatusCode(size_t Code){
	if (_StatusCode == 0)
	_StatusCode = Code;
}

bool Request::getBodyRead() const
{
	return _bodyRead;
}

void Request::setBodyRead(bool bodyRead)
{
	_bodyRead = bodyRead;
}

std::string Request::getRequestURI() const{
	return _headers.find("URI")->second;
}

Request& Request::operator=(const Request& request){
	_StatusCode = request._StatusCode;
	_bodyRead = request._bodyRead;
	_REQ = request._REQ;
	_lastHeaderPos = request._lastHeaderPos;
	_parsePos = request._parsePos;
	_contentLength = request._contentLength;
	_Boundaries = request._Boundaries;
	_headers = request._headers;
	_body = request._body;
	return *this;
}

void	Request::toString(){


	std::cout << "\n\%\%\%\%\%\%\%\% Request \%\%\%\%\%\%\%\%\%%\n" << std::endl;
	std::cout << _REQ << std::endl;
		
	puts("\n\%\%\%\%\%\%\%\% Headers \%\%\%\%\%\%\%\%\%%\n");
    for(std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
        std::cout << "[" << it->first << "]--[" << it->second << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Body \%\%\%\%\%\%\%\%\%%\n");
	std::cout << "[" << _body << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Boundaries \%\%\%\%\%\%\%\%\%%\n");
	for (std::vector<BoundRequest>::iterator it = _Boundaries.begin(); it != _Boundaries.end(); ++it){
		for (std::map<std::string, std::string>::const_iterator it2 = it->_headers.begin(); it2 != it->_headers.end(); ++it2){
			std::cout << "[" << it2->first << "]--[" << it2->second << "]" << std::endl;
			std::cout << "[" << it->_body << "]\n\n\n" << std::endl;	
		}
	}
}

