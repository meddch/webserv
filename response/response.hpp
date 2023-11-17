/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:01:48 by azari             #+#    #+#             */
/*   Updated: 2023/11/17 15:27:51 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include <unordered_map>
#include "../request/Request.hpp"

class Response
{
	private:
		size_t		_statusCode;
		std::string _version;
		std::string _status;
		std::string _body;
		std::string _reasonPhrase;
		std::string _contentLength;
		bool		_KEEPALIVE;
		std::unordered_map<std::string, std::string> _headers;
	public:

		Response();
		std::string _response;
		std::string generateResponse(Request& request);
		std::string getResponse() const;
		std::string findMimeType(std::string extention);
		std::string generateStatusPhrase(size_t status);
		std::string generateResponseDate();
		bool handleResponseError(Request& request);
		void initResponseHeaders();
		bool isConnectionKeepAlive();
		void setStatusCode(size_t statusCode);
};

