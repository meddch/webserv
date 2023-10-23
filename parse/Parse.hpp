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
#define DEFAULT_PORT	80
#define DEFAULT_INDEX	"index.html"


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
	string					root;
    std::pair<int, string> 	redirect;
	bool					cgi;
	string					cgiPath;
	string					cgiExtension;

};

struct  ServerContext
{
    string					root;
	Listen_Addr		        address;
	stringVec				index;
	string					serverName;
	stringVec				allowedMethods;
	ssize_t					clientMaxBodySize;
	map<int, string>		errorPages;
	vector<LocationContext>	locations;

};



class Parse
{
	private :
		deque<string> _tokens;
		vector<ServerContext> _config;


		stringVec validServerKeys;
		stringVec validLocationKeys;
		
	public :

		//Constructor,Geters
		Parse(string const &filename);
		void  C_validServerKeys();
		void C_validLocationKeys();


		vector<ServerContext> GetConfig();

	private :
		//Parse utils
		void	GetTokens(const string& content);
		string	Accept(void);
		void	Skip(const string& token);
		
		bool isValidServerKey(string key);
		bool isValidLocationKey(string key);

		ServerContext   Server(void);
		LocationContext Location(void);

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
		void ParseCgi(LocationContext& location);
		void ParseCgiPath(LocationContext& location);
		void ParseCgiExtension(LocationContext& location);
		
		
		
		void addDefaultLocation(ServerContext& server);



};