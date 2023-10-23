/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:12:03 by azari             #+#    #+#             */
/*   Updated: 2023/10/23 15:14:30 by azari            ###   ########.fr       */
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
        std::string _URI;
        std::string _URIQueries;
        std::string _HttpRequestMethod;
        std::string _HttpVersion;
        bool _POST;
	protected :
        std::map <std::string, std::string> _headers;
		std::map<std::string, std::string> _formdata;
		std::string _body;
        std::string _REQ;
        size_t _lastHeaderPos;
        size_t _errorCode;
        size_t _parsePos;
	
    public:
        Request(std::string _REQ);
        ~Request();
        void parseRequest();
        void parseRequestLine(std::string requestLine);
        void parseRequestBody();
		void parseRequestHeaders();
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