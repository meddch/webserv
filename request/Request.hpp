/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:12:03 by azari             #+#    #+#             */
/*   Updated: 2023/10/24 13:46:55 by azari            ###   ########.fr       */
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
        std::map <std::string, std::string> _headers;
		std::map<std::string, std::string> _formdata;
		std::string _requestMethod;
		std::string _body;
        std::string _REQ;
        size_t _lastHeaderPos;
        size_t _parsePos;
		size_t _contentLength;
    public:
        Request(std::string _REQ);
        ~Request();
        void parseRequest();
        void parseRequestLine(std::string requestLine);
        void parseRequestBody();
		void parseRequestHeaders();
		void toString();
		std::string getRequestMethod() const;
		std::string getRequestString() const;
		size_t getContentLength() const;
		size_t getLastHeaderPos() const;
		size_t getBodyPosition() const;
		size_t getErrorCode() const;
		void setRequestMethod(std::string requestMethod);
		void setRequestString(std::string requestString);
		void setContentLength(size_t contentLength);
		void setLastHeaderPos(size_t lastHeaderPos);
		void setErrorCode(size_t errorCode);
};

class BoundRequest : public Request
{
	public:
		std::string _boundary;
		std::string _filename;
		std::string _name;
		size_t _boundaryEndPos;
		size_t _boundaryNextPos;
	public:
		BoundRequest(std::string _boundary, std::string _REQ);
};