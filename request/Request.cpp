/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 15:21:09 by azari             #+#    #+#             */
/*   Updated: 2023/10/23 17:15:39 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::~Request(){
	_headers.clear();
}

Request::Request(std::string _request):  _POST(false), _REQ(_request), _lastHeaderPos(0), _errorCode(0), _parsePos(0){
	_Boundaries.clear();
}


BoundRequest::BoundRequest(std::string boundary, std::string _REQ): Request(_REQ), _boundary(boundary), _boundaryEndPos(0), _boundaryNextPos(0){
	_headers.clear();
}

void Request::parseRequest(){

    // Parsing Request line
	std::cout << _REQ << std::endl;
	parseRequestLine(_REQ.substr(0,_REQ.find("\r\n")));
	if (_errorCode)
	    return ;

    // parsing Request Headers
	parseRequestHeaders();
	if (_errorCode)
	    return ;
    // parsing Request Body
    if (_POST)
        parseRequestBody();

	puts("\n\%\%\%\%\%\%\%\% Headers \%\%\%\%\%\%\%\%\%%\n");
    for(std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
        std::cout << "[" << it->first << "]--[" << it->second << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Body \%\%\%\%\%\%\%\%\%%\n");
	std::cout << "[" << _body << "]" << std::endl;

	puts("\n\%\%\%\%\%\%\%\% Boundaries \%\%\%\%\%\%\%\%\%%\n");
	for (std::vector<BoundRequest>::iterator it = _Boundaries.begin(); it != _Boundaries.end(); ++it){
		std::cout << "[" << it->_name << "]--[" << it->_filename << "]" << std::endl;
		std::cout << "[" << it->_body << "]" << std::endl;
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
    for (i = 0; i < 3; i++)
        if (methods[i] == _headers["Method"])
            break;
    (i >= 3) && (_errorCode = 501); // 501: Method Not Implemented
    (i == 1) && (_POST = true);
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
		_headers[headerKey] = headerValue;
	    _parsePos = _REQ.find("\r\n", _parsePos) + 2;
	}
	_parsePos = _lastHeaderPos + 4;
}


void    Request::parseRequestBody(){
    
    std::string key, value;
	if (_headers.find("content-length") != _headers.end() && _headers.find("Transfer-Encoding") == _headers.end() && _headers["content-length"] != "0")
    	_body =_REQ.substr(_parsePos, std::stoi(_headers["content-length"]));
    else if (_headers["content-type"].find("multipart/form-data") != std::string::npos){
        
        BoundRequest obj("--" + _headers["content-type"].substr(_headers["content-type"].find("boundary=") + 9, _headers["content-type"].npos), _REQ);
		obj._boundaryEndPos = _REQ.find(obj._boundary + "--");
		
		while (_parsePos < obj._boundaryEndPos){
			obj._parsePos = _REQ.find(obj._boundary, _parsePos) + obj._boundary.length() + 2;
			obj._lastHeaderPos = _REQ.find("\r\n\r\n", _parsePos);
			obj._boundaryNextPos = _REQ.find(obj._boundary , _parsePos);
			parseRequestHeaders();

			if (obj._headers.find("Content-Disposition") != obj._headers.end() && obj._headers["Content-Disposition"].find("filename=") != std::string::npos){
				obj._filename = obj._headers["Content-Disposition"].substr(obj._headers["Content-Disposition"].find("filename=") + 10, obj._headers["Content-Disposition"].npos - 11);
				obj._name = obj._headers["Content-Disposition"].substr(obj._headers["Content-Disposition"].find("name=") + 6, obj._headers["Content-Disposition"].find(";") - obj._headers["Content-Disposition"].find("name=") - 7);
			}
			else if (obj._headers.find("Content-Disposition") != obj._headers.end() && obj._headers["Content-Disposition"].find("name=") != std::string::npos)
				obj._name = obj._headers["Content-Disposition"].substr(obj._headers["Content-Disposition"].find("name=") + 6, obj._headers["Content-Disposition"].npos - 7);
				
			while (obj._parsePos < obj._boundaryNextPos){
				obj._body += _REQ[obj._parsePos];
				obj._parsePos++;
			}
			_Boundaries.push_back(obj);
			_parsePos = obj._boundaryNextPos;
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
