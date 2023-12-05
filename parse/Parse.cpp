#include "Parse.hpp"
#include "utils.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sys/_types/_ssize_t.h>
#include <sys/stat.h>


// static helpers *******************************************

std::string ServerKeys[] = {"root", "server_name", "listen", "client_max_body_size", "error_page", "location", "upload"};

std::string LocationKeys[] = {"autoindex", "alias", "allowed_methods", "index", "return", "root", "upload_path", "cgi_path"};


void  Parse::C_validServerKeys()
{
  for (size_t i = 0; i < 7; i++)
    validServerKeys.push_back(ServerKeys[i]);
}

void Parse::C_validLocationKeys()
{
  for (size_t i = 0; i < 8; i++)
    validLocationKeys.push_back(LocationKeys[i]);
}


bool Parse::isValidServerKey(std::string key)
{
	bool state = std::find(validServerKeys.begin(), validServerKeys.end(), key) == validServerKeys.end();
	return state;
}

bool Parse::isValidLocationKey(std::string key)
{
	return std::find(validLocationKeys.begin(), validLocationKeys.end(), key) == validLocationKeys.end();
}




//Constructor,Geters ******************************************

Parse::Parse(std::string const &filename)
{
	_config.clear();
    _tokens.clear();
	C_validServerKeys();
	C_validLocationKeys();

    std::ifstream file(filename.c_str());
	if (file.fail())
    	throw std::runtime_error("Could not open file: " + filename);

	std::string line;
	std::string content;
	while (std::getline(file, line)) 
    	content += line;

	if (content.empty())
    	throw std::runtime_error("Empty file: " + filename);

	GetTokens(content);
    while(!_tokens.empty())
        ParseServer();
	if (_config.empty())
		throw std::runtime_error("Config Error : " + filename);
}

std::vector<ServerContext> Parse::GetConfig()
{
    return _config;
}


// Parsiing utils  >> *********************************************

void	Parse::GetTokens(const std::string& content)
{
    const std::string whitespaces(" \t\r\n"), spc_caratcter("{};");
	size_t pos = 0;

	while (pos < content.size())
	{
		if (spc_caratcter.find(content[pos]) != spc_caratcter.npos)
		{
			_tokens.push_back(std::string(1, content[pos]));
			pos++;
		}
		else if (whitespaces.find(content[pos]) != whitespaces.npos)
			pos++;
		else
		{
			size_t tokEnd = content.find_first_of(whitespaces + spc_caratcter, pos);
			if (tokEnd == std::string::npos)
				tokEnd = content.size();
			_tokens.push_back(content.substr(pos, tokEnd - pos));
			pos += _tokens.back().size();
		}
	}
}

std::string	Parse::Accept(void)
{
	if (_tokens.empty())
		throw std::runtime_error("Error : Parser::Syntax error!");

	std::string token = _tokens.front();
	_tokens.pop_front();
	return token;
}

void	Parse::Skip(const std::string& token)
{
	if (_tokens.front() != token)
		throw std::runtime_error("Error : Parser::Can't find " + token + "!");

	_tokens.pop_front();
}

//********************************************************************

ServerContext Parse::Server(void)
{
	ServerContext config;

    config.address.port = -1; 
	config.root.clear();
	config.serverName.clear();
	config.errorPages.clear();
	config.clientMaxBodySize = MAX_BODY_SIZE;
	config.upload = false;

	return config;
}

void	Parse::ParseServer()
{
	ServerContext server = Server();
	Skip("server"), Skip("{");

	std::string token;
	while ((token = Accept()) != "}")
	{
		if (isValidServerKey(token))
			throw std::runtime_error("Error : Parser::unknown  key " + token + "!");
		else if (token == "root")
			ParseServerRoot(server);
		else if (token == "listen")
			ParseAddress(server);
		else if (token == "server_name")
			ParseServerName(server);
		else if (token == "client_max_body_size")
			ParseClientMaxBodySize(server);
		else if (token == "error_page")
			ParseErrorPage(server);
		else if (token == "location")
			ParseLocation(server);
		else if (token == "upload")
			ParseUpload(server);
	}

	if (server.serverName.empty())
		return;
	if (server.root.empty())
		server.root = realpath(".", NULL) + std::string(ROOT);

	if (server.address.port == -1)
		server.address.port = DEFAULT_PORT;


	// Check if server name already exists and port matches
	std::vector<ServerContext>::iterator it;
	for (it = _config.begin(); it != _config.end(); it++)
	{
		if (it->serverName == server.serverName && it->address.port == server.address.port)
			throw std::runtime_error("Parser: duplication server name " + it->serverName + "!");
	}


	
	addDefaultLocation(server);
	_config.push_back(server);
}

LocationContext Parse::Location(void)
{
	LocationContext location;

	location.allowedMethods.clear();
	location.index.clear();
	location.alias.clear();
	location.redirect.second.clear();
	location.redirect.first = 0;
	location.autoindex = false;
	location.cgiPath.clear();
	location.uploadPath.clear();


	return location;
}

void Parse::ParseLocation(ServerContext& server)
{
	LocationContext location = Location();
	ParseUri(location), Skip("{");

	// Check if location uri already exists
	std::vector<LocationContext>::iterator it;
	for (it = server.locations.begin(); it != server.locations.end(); it++) 
		if (it->uri == location.uri)
			throw std::runtime_error("Parser: duplication location " + it->uri + "!");

	std::string token;
	while ((token = Accept()) != "}")
    {
		if (isValidLocationKey(token))
			throw std::runtime_error("Error : Parser::Unknown key " + token + "!");
		else if (token == "autoindex")
			ParseAutoindex(location);
		else if (token == "alias")
			ParseAlias(location);
		else if (token == "allowed_methods")
			ParseAllowedMethods(location);
		else if (token == "index")
			ParseIndex(location);
		else if (token == "return")
            ParseRedirect(location);
		else if (token == "root")
			ParseLocationRoot(location);
		else if (token == "cgi_path")
			ParseCgiPath(location);
		else if (token == "upload_path")
			ParseUploadPath(location);
	}

	if (location.allowedMethods.empty())
		location.allowedMethods.push_back("GET");

	server.locations.push_back(location);
}


void Parse::ParseServerRoot(ServerContext& server)
{
    
	std::string path = Accept();
	if (!server.root.empty())
	{
		Skip(";");
		return;
	}
	server.root = path;
	Skip(";");
}

void Parse::ParseLocationRoot(LocationContext& location)
{
	std::string path = Accept();
	if (!location.alias.empty())
	{
		Skip(";");
		return;
	}
	location.root = path;
	Skip(";");
}

void Parse::ParseServerName(ServerContext& server)
{
	if (!server.serverName.empty())
	{
		Accept();
		Skip(";");
		return;
	}

    server.serverName = Accept();
	Skip(";");
}

void Parse::ParseAddress(ServerContext& server)
{
   if (server.address.port != -1)
   {
		Accept();
		Skip(";");
		return;
	}
	
	try
    {
		std::string token = Accept();
		size_t colonPos = token.find(":");
		
		if (colonPos != std::string::npos)
        {
			server.address.ip = toIPv4(token.substr(0, colonPos));
			token.erase(token.begin(), token.begin() + colonPos + 1);
		}
		else 
			server.address.ip = 0;

		int port = toNum(token);
		if (port <= 0 || port > 65535)
			throw std::runtime_error("Parser : Invalid Port!");
		server.address.port = port;

		Skip(";"); 
	}
	catch (...)
    {
		throw std::runtime_error("Parser: invalid listen!");
	}
}

void Parse::ParseClientMaxBodySize(ServerContext& server)
{
    if (server.clientMaxBodySize != 100000000)
	{
		Accept();
		Skip(";");
		return;
	}
	try
    {
		ssize_t value = toNum(Accept());
		if (value < 0)
			throw std::runtime_error("negative value");

		
		if (value < 100000000)
			server.clientMaxBodySize = value;
		Skip(";");
	}
	catch (std::exception& e)
    {
		throw std::runtime_error("Parser: body size, " + std::string(e.what()) + "!");
	}
}


void	Parse::ParseUpload(ServerContext &server)
{
	std::string token = Accept();
	if (token != "on" && token != "off")
		throw std::runtime_error("Parser: invalid upload value!");

	server.upload = token == "on" ? true : false;
	Skip(";");
}

void Parse::ParseErrorPage(ServerContext& server)
{
    std::vector<std::string> tokens;
	std::string token;

	while ((token = Accept()) != ";")
        tokens.push_back(token);

	if (tokens.size() < 2) 
		throw std::runtime_error("Parser: syntax error!");

	try {
		for (size_t i = 0; i < tokens.size() - 1; i++)
        {
			int code = toNum(tokens[i]);
			std::map<int, std::string>::iterator it;
			if (server.errorPages.find(code) != server.errorPages.end())
				continue ;
			server.errorPages.insert(std::make_pair(code, tokens.back()));
		}
	}
	catch (std::exception& e)
    {
		throw std::runtime_error("Parser: error page, " + std::string(e.what()) + "!");
	}
}


void Parse::ParseUri(LocationContext& location)
{
	
    location.uri = Accept();
	if (location.uri.empty() || (location.uri[0] != '/'  && location.uri[0] != '.'))
		throw std::runtime_error("Parser: invalid uri!");
	else if (location.uri.size() > 1 && location.uri[0] == '/' && location.uri[1] == '/')
		throw std::runtime_error("Parser: invalid uri!");
	
}



void Parse::ParseAutoindex(LocationContext& location)
{
	std::string token = Accept();
	if (token != "on" && token != "off")
		throw std::runtime_error("Parser: invalid autoindex value!");

	location.autoindex = token == "on" ? true : false;
	Skip(";");
}

void Parse::ParseAlias(LocationContext& location)
{
	std::string token = Accept();
	if (!location.alias.empty())
	{
		Skip(";");
		return;
	}
	location.alias = token;
	Skip(";");

}

void Parse::ParseAllowedMethods(LocationContext& location)
{
	std::string token;
	while ((token = Accept()) != ";")
    {
		if (token != "GET" && token != "POST" && token != "DELETE" ) 
			throw std::runtime_error("Error Parser::unknown method " + token);

		location.allowedMethods.push_back(token);
	}
}

void Parse::ParseIndex(LocationContext& location)
{
	std::string token = Accept();
	if (!location.index.empty())
	{
		Skip(";");
		return;
	}
	location.index = token;
	Skip(";");

}

void Parse::ParseRedirect(LocationContext& location)
{
	try 
    {
		location.redirect.first = toNum(Accept());
		location.redirect.second = Accept(); 
	}
	catch (std::exception& e) 
    {
		throw std::runtime_error("Parser: " + std::string(e.what()) + "!");
	}
	Skip(";");
}





void Parse::addDefaultLocation(ServerContext& server)
{
	for (std::vector<LocationContext>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
		if (it->uri == "/")
			return;

	LocationContext location = Location();
	location.uri = "/";
	location.root = server.root;
	location.allowedMethods.push_back("GET");

	server.locations.push_back(location);
}


void Parse::ParseCgiPath(LocationContext& location)
{
	std::string token = Accept();
	if (!location.cgiPath.empty())
	{
		Skip(";");
		return;
	}
	location.cgiPath = token;
	Skip(";");
}

void Parse::ParseUploadPath(LocationContext& location)
{
	std::string token = Accept();
	if (!location.uploadPath.empty())
	{
		Skip(";");
		return;
	}
	location.uploadPath = token;
	Skip(";");
}

