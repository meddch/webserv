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
#define ROOT			"Home"

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
		
	public :

		//Constructor,Geters
		ParseConfig(string const &filename);
		vector<ServerContext> GetConfig();

	private :
		//Parse utils
		void	GetTokens(const string& content);
		string	Accept(void);
		void	Skip(const string& token);
		
		static bool isValidServerKey(string key);
		static bool isValidLocationKey(string key);

		ServerContext   CreateServer(void);
		LocationContext CreateLocation(void);

		void ParseServer(void);
		void ParseLocation(ServerContext& server);

		void ParseRoot(ServerContext& server);
		void ParseServerName(ServerContext& server);
		void ParseAddress(ServerContext& server);
		void ParseClientMaxBodySize(ServerContext& server);
		void ParseErrorPage(ServerContext& server);
		
		void ParseUri(LocationContext& location);
		void ParseAutoindex(LocationContext& location);
		void ParseAlias(LocationContext& location);
		void ParseAllowedMethods(LocationContext& location);
		void ParseIndex(LocationContext& location);
		void ParseRedirect(LocationContext& location);
		
		
		void addDefaultLocation(ServerContext& server);



};