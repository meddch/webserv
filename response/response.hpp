/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:01:48 by azari             #+#    #+#             */
/*   Updated: 2023/12/05 19:48:42 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <sys/_types/_off_t.h>
#include <sys/_types/_ssize_t.h>
#include <unordered_map>
#include "../request/Request.hpp"
#include "../parse/utils.hpp"
#include <dirent.h>
#include <sys/stat.h>

#define BYTES 1000
class Response
{
	private:
		size_t			_statusCode;
		std::string 	_version;
		std::string 	_status;
		std::string 	_body;
		std::string 	_reasonPhrase;
		bool			_KEEPALIVE;
		bool			_isCGI;
		bool			_isAutoIndex;
		bool			_isDirectory;
		bool			_isFile;
		bool			_isMethodAllowed;
		bool 			_isfileRead;
		std::unordered_map<std::string, std::string> _headers;
	public:
		Response();
		std::string 	responseString;
		std::string 	_contentLength;
		std::string 	response;
		std::string 	filebody;
		std::string 	filePath;
		std::string 	uploadFilePath;
		std::string		root;
		bool 			_headerSent;
		bool			readyToSend;
		int				fd;
		ssize_t			_fileSize;
		ssize_t			_offset;
		ssize_t			_contentLengthInt;
		std::string generateResponse(Request&);
		std::string findMimeType(std::string);
		std::string generateStatusPhrase(size_t);
		std::string generateResponseDate();
		bool handleResponseError(Request&);
		void initResponseHeaders(Request&);
		bool handleResponseError(Request& request, std::string errorPage, std::string code);
		void initRedirectHeaders(Request& request, std::string location);
		void generateAutoIndex(Request&);
		bool isConnectionKeepAlive();
		void setStatusCode(size_t);
		bool isfileRead();
		void generateChunkedResponse();

};

struct t_path
{
	std::string serverRoot;
	std::string routedPath;
	std::string alias;
	std::string uri;
};