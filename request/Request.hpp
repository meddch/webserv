/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:12:03 by azari             #+#    #+#             */
/*   Updated: 2023/12/04 15:40:56 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sys/socket.h>

class BoundRequest;
class Request
{
    private:
        size_t	_StatusCode;
		bool  	_bodyRead;
		bool  	_hostExist;
		bool  	_transferEncodingExist;
		bool  	_chunked;
		bool	_contentLengthExist;
	public:
		bool 		_reqParsed;
		std::string uri;
	protected :
		std::string 	_requestMethod;
        std::string 	_REQ;
        size_t 			_lastHeaderPos;
        size_t 			_parsePos;
		ssize_t 		_contentLength;
    public:
        std::vector<BoundRequest> 			_Boundaries;
        std::map <std::string, std::string> _headers;
		std::map<std::string, std::string> 	_formdata;
		std::string 						_body;
        Request();
        ~Request();
        void parseRequest();
        void parseRequestLine(std::string requestLine);
        void parseRequestBody();
		void parseRequestHeaders();
		void markExistance(const std::string&);
		void checkExistance(void);
		std::string parseURI(std::string);
		std::string getRequestMethod() const;
		std::string getRequestString() const;
		ssize_t getContentLength() const;
		size_t getLastHeaderPos() const;
		size_t getBodyPosition() const;
		size_t getStatusCode() const;
		void setRequestMethod(std::string requestMethod);
		void setRequestString(std::string requestString);
		void setContentLength(size_t contentLength);
		void setLastHeaderPos(size_t lastHeaderPos);
		void isMethodAllowed(std::vector<std::string>);
		void setStatusCode(size_t Code);
		bool getBodyRead() const;
		void setBodyRead(bool bodyRead);
		std::string getRequestURI() const;
		Request& operator=(const Request& other);
};

class BoundRequest : public Request
{
	public:
		std::string _boundary;
		std::string _filename;
		std::string _name;
		size_t _boundaryEndPos;
		size_t _boundaryNumber;
		size_t _BoundaryNext;
	public:
		BoundRequest(std::string _boundary, std::string _REQ);
};
