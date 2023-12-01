/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:01:48 by azari             #+#    #+#             */
/*   Updated: 2023/12/01 15:41:41 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include "../request/Request.hpp"
#include <dirent.h>
#include <sys/stat.h>

class Response
{
	private:
		size_t		_statusCode;
		std::string _version;
		std::string _status;
		std::string _body;
		std::string _reasonPhrase;
		bool		_KEEPALIVE;
		bool		_isCGI;
		bool		_isAutoIndex;
		bool		_isDirectory;
		bool		_isFile;
		bool		_isMethodAllowed;
		bool 		_isfileRead;
		std::unordered_map<std::string, std::string> _headers;
	public:
		std::string _contentLength;

		Response();
		std::string 	response;
		std::string 	filebody;
		std::string 	filePath;
		std::string 	responseString;
		bool 			_headerSent;
		bool			readyToSend;
		int				fd;
		std::string		root;
		std::string generateResponse(Request&);
		std::string findMimeType(std::string);
		std::string generateStatusPhrase(size_t);
		std::string generateResponseDate();
		bool handleResponseError(Request&);
		void initResponseHeaders(Request&);
		bool handleResponseError(Request& request, std::string code);
		void generateAutoIndex(Request&);
		bool isConnectionKeepAlive();
		void setStatusCode(size_t);
		bool isfileRead();
};


