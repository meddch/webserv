/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azari <azari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:01:48 by azari             #+#    #+#             */
/*   Updated: 2023/11/09 15:27:25 by azari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <map>
#include "../Request/request.hpp"

class Response
{
	private:
		std::string _response;
		std::string _version;
		std::string _status;
		std::string _body;
		std::string _reasonPhrase;
		std::map<std::string, std::string> _headers;
	public:
		
		Response();
		void generateResponse(Request& request);
		std::string getResponse() const;
		std::string findMimeType(std::string extention);
};

