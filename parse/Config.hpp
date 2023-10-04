#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <deque>
#include <exception>
#include <fstream>
#include <netinet/in.h>
#include <limits.h>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <sys/stat.h>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;
using std::map;
using std::set;
using std::deque;
using std::vector;
using std::string;
using std::ostream;
using std::exception;
using std::runtime_error;


typedef struct pollfd PollFd;
typedef struct stat Stat;
typedef map<string, string> StringMap;
typedef vector<string> stringVec;



struct Address
{
	in_addr_t ip;
	int port;
	bool operator<(const Address& rhs) const
	{
		if (ip == rhs.ip)
			return port < rhs.port;
		return ip < rhs.ip ? true : false;
	}
};

struct LocationConf
{
   bool autoindex;
	string uri;
	string alias;
	stringVec allowedMethods;
	stringVec index;
	std::pair<int, string> redirect;

};

struct  ServerContext
{
    string root;
	Address address;
	stringVec index;
	string serverName;
	ssize_t clientMaxBodySize;
	map<int, string> errorPages;
	vector<LocationConf> locations;
};

class	Config
{
public:
	/* Main Parser */
	vector<ServerContext> parse(const string& filename);

	/* Validation */
	static bool isValidErrorCode(int code);
	static bool isValidRedirectCode(int code);
	static bool isValidServerKey(string key);
	static bool isValidLocationKey(string key);

private:
	// Logic
	void lex(const string& content, const string& whitespace, const string& symbol);

	string accept(void);
	void consume(const string& token);

	/* Context Parsers */
	void parseServer(void);
	void parseLocation(ServerContext& server);

	/* Server Field Parsers */
	void parseRoot(ServerContext& server);
	void parseServerName(ServerContext& server);
	void parseAddress(ServerContext& server);
	void parseClientMaxBodySize(ServerContext& server);
	void parseErrorPage(ServerContext& server);

	/* Location Field Parsers */
	void parseUri(LocationConf& location);
	void parseAutoindex(LocationConf& location);
	void parseAlias(LocationConf& location);
	void parseAllowedMethods(LocationConf& location);
	void parseCgi(LocationConf& location);
	void parseIndex(LocationConf& location);
	void parseRedirect(LocationConf& location);

	/* Keys */
	static const stringVec validServerKeys;
	static const stringVec validLocationKeys;
	static const vector<int> validErrorCodes;
	static const vector<int> validRedirectCodes;

	/* Default Settings */
	ServerContext createServer(void);
	LocationConf createLocation(void);
	void addDefaultErrorPages(ServerContext& server);
	void addDefaultLocation(ServerContext& server);

	/* Member Data */
	vector<ServerContext> m_configs;
	deque<string> m_tokens;
};

     
#define DEFAULT_400		"public/default_error/400.html"
#define DEFAULT_404		"public/default_error/404.html"
#define DEFAULT_CONF	"config/default.conf"
#define ROOT			"public"
#define CGI_BIN			"/cgi-bin/"
#define PY_PATH			"/usr/bin/python3"
#define PHP_PATH		"/usr/bin/php"

#define HTTP_VERSION	"HTTP/1.1"
#define SERVER_SOFTWARE	"Webserv42/1.0"

#define RECV_SIZE		4096
#define CLIENT_TIMEOUT	60

#define RESET	"\033[0m"
#define CYAN	"\033[36m"
#define YELLOW	"\033[33m"
#define RED		"\033[31m"
#define ORANGE	"\033[38;5;214m"
#define DORANGE "\033[37;5;166m"

#define PADDING	21
#define DEBUG	4
#define INFO	3
#define WARNING	2
#define ERROR	1

#define LOG_DISPLAY_LEVEL	DEBUG
