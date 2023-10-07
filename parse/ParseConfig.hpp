/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mechane <mechane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 10:31:01 by mechane           #+#    #+#             */
/*   Updated: 2023/10/07 12:27:50 by mechane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//Containers
#include <set>
#include <map>
#include <deque>
#include <string>
#include <vector>
#include <algorithm>

//Streams
#include <iostream>
#include <fstream>
#include <sstream>

//execption
#include <exception>
#include <stdexcept>
#include <limits.h>

#include <netinet/in.h>
#include <sys/stat.h>

using std::set;
using std::map;
using std::endl;
using std::cout;
using std::cerr;
using std::deque;
using std::string;
using std::vector;
using std::ostream;
using std::exception;
using std::runtime_error;

//Typdef
typedef map<string, string> stringMap;
typedef vector<string> stringVec;



#define WHITESPACES 	" /t/n"
#define ROOT			"public"

struct Listen_Addr
{
	in_addr_t	ip;
	int			port;

	bool operator<(const Listen_Addr& rhs) const
	{
		if (ip == rhs.ip)
			return port < rhs.port;
		return ip < rhs.ip ? true : false;
	}
};

//Context Data

struct LocationContext
{
    bool					autoindex;
	string					uri;
	string					alias;
	stringVec				allowedMethods;
	stringVec				index;
    std::pair<int, string> 	redirect;

};

struct  ServerContext
{
    string					root;
	Listen_Addr		        address;
	stringVec				index;
	string					serverName;
	ssize_t					clientMaxBodySize;
	map<int, string>		errorPages;
	vector<LocationContext>	locations;
};



class ParseConfig
{
	private :
		deque<string> _tokens;
		vector<ServerContext> _config;


		static const stringVec validServerKeys;
		static const stringVec validLocationKeys;
		static const vector<int> validErrorCodes;
		static const vector<int> validRedirectCodes;	
		
	public :
		//Constructor,Geters
		ParseConfig(string const &filename);
		vector<ServerContext> GetConfig();

	private :
		//Parse utils
		void	GetTokens(const string& content);
		ServerContext CreateServer(void);
		void	Skip(const string& token);
		string	Accept(void);
		void	ParseServer(void);
		
		static bool isValidErrorCode(int code);
		static bool isValidRedirectCode(int code);
		static bool isValidServerKey(string key);
		static bool isValidLocationKey(string key);

		void ParseLocation(ServerContext& server);

		/* Server Field Parsers */
		void ParseRoot(ServerContext& server);
		void ParseServerName(ServerContext& server);
		void ParseAddress(ServerContext& server);
		void ParseClientMaxBodySize(ServerContext& server);
		void ParseErrorPage(ServerContext& server);
		
		/* Location Field Parsers */
		void ParseUri(LocationContext& location);
		void ParseAutoindex(LocationContext& location);
		void ParseAlias(LocationContext& location);
		void ParseAllowedMethods(LocationContext& location);
		void ParseIndex(LocationContext& location);
		void ParseRedirect(LocationContext& location);
		
		/* Keys */
		static const stringVec validServerKeys;
		static const stringVec validLocationKeys;
		static const vector<int> validErrorCodes;
		static const vector<int> validRedirectCodes;
		
		/* Default Settings */
		LocationContext CreateLocation(void);
		void addDefaultErrorPages(ServerContext& server);
		void addDefaultLocation(ServerContext& server);


};