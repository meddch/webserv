/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:12:03 by azari             #+#    #+#             */
/*   Updated: 2023/10/24 21:16:42 by azari            ###   ########.fr       */
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
        std::vector<BoundRequest> _Boundaries;
        size_t _errorCode;
		bool   _bodyRead;
	protected :
		std::map<std::string, std::string> _formdata;
		std::string _requestMethod;
		std::string _body;
        std::string _REQ;
        size_t _lastHeaderPos;
        size_t _parsePos;
		ssize_t _contentLength;
    public:
        std::map <std::string, std::string> _headers;
        Request();
        ~Request();
        void parseRequest();
        void parseRequestLine(std::string requestLine);
        void parseRequestBody();
		void parseRequestHeaders();
		void toString();
		std::string getRequestMethod() const;
		std::string getRequestString() const;
		ssize_t getContentLength() const;
		size_t getLastHeaderPos() const;
		size_t getBodyPosition() const;
		size_t getErrorCode() const;
		void setRequestMethod(std::string requestMethod);
		void setRequestString(std::string requestString);
		void setContentLength(size_t contentLength);
		void setLastHeaderPos(size_t lastHeaderPos);
		void setErrorCode(size_t errorCode);
		bool getBodyRead() const;
		void setBodyRead(bool bodyRead);
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