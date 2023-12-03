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


//Typdef
typedef std::map<std::string, std::string> stringMap;
typedef std::vector<std::string> stringVec;

//Constants
#define DEFAULT_PORT	80
#define DEFAULT_INDEX	"index.html"
#define ROOT			"/Session"
#define NONE			"none"
#define MAX_BODY_SIZE	1000000000000


//Structs
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
    bool						autoindex;
	std::string					uri;
	std::string					alias;
	std::string					uploadPath;
	stringVec					allowedMethods;
	stringVec					index;
	std::string					root;
	std::string					cgiPath;
    std::pair<int, std::string> redirect;

};

struct  ServerContext
{
    std::string						root;
	bool							upload;
	Listen_Addr		        		address;
	std::string						serverName;
	ssize_t							clientMaxBodySize;
	std::map<int, std::string>		errorPages;
	std::vector<LocationContext>	locations;

};

//Parse Class

class Parse
{

	private :
		std::deque<std::string> _tokens;
		std::vector<ServerContext> _config;


		stringVec validServerKeys;
		stringVec validLocationKeys;
		
	public :

		//Constructor,Geters
		Parse(std::string const &filename);
		void  C_validServerKeys();
		void C_validLocationKeys();


		std::vector<ServerContext> GetConfig();

	private :
		//Parse utils
		void		GetTokens(const std::string& content);
		void		Skip(const std::string& token);
		std::string	Accept(void);
		
		bool isValidServerKey(std::string key);
		bool isValidLocationKey(std::string key);

		ServerContext   Server(void);
		LocationContext Location(void);

		void ParseServer(void);
		void ParseLocation(ServerContext& server);

		void ParseServerRoot(ServerContext& server);
		void ParseServerName(ServerContext& server);
		void ParseAddress(ServerContext& server);
		void ParseClientMaxBodySize(ServerContext& server);
		void ParseErrorPage(ServerContext& server);
		void ParseUpload(ServerContext &server);

		
		void ParseUri(LocationContext& location);
		void ParseLocationRoot(LocationContext& location);
		void ParseAutoindex(LocationContext& location);
		void ParseAlias(LocationContext& location);
		void ParseAllowedMethods(LocationContext& location);
		void ParseIndex(LocationContext& location);
		void ParseRedirect(LocationContext& location);
		
		void addDefaultLocation(ServerContext& server);

		void ParseCgiPath(LocationContext& location);
		void ParseUploadPath(LocationContext& location);


};