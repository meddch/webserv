#include "Parse.hpp"
#include "Utils.hpp"


// static helpers *******************************************

std::string ServerKeys[] = {"root", "server_name", "listen", "client_max_body_size", "error_page", "location", "upload"};

std::string LocationKeys[] = {"autoindex", "alias", "allowed_methods", "index", "return", "root", "cgi_path", "upload_path"};


void  Parse::C_validServerKeys()
{
  for (size_t i = 0; i < sizeof(ServerKeys) / sizeof(ServerKeys[0]); i++)
    validServerKeys.push_back(ServerKeys[i]);
}

void Parse::C_validLocationKeys()
{
  for (size_t i = 0; i < sizeof(LocationKeys) / sizeof(LocationKeys[0]); i++)
    validLocationKeys.push_back(LocationKeys[i]);
}


bool Parse::isValidServerKey(std::string key)
{
	return std::find(validServerKeys.begin(), validServerKeys.end(), key) == validServerKeys.end();
}

bool Parse::isValidLocationKey(std::string key)
{
	return std::find(validLocationKeys.begin(), validLocationKeys.end(), key) == validLocationKeys.end();
}




//Constructor,Geters ******************************************

Parse::Parse(std::string const &filename)
{
    std::ifstream file(filename.c_str());
	if (file.fail())
		throw std::runtime_error("Failed to open file: " + filename);

    
	_config.clear();
    _tokens.clear();
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.empty())
		throw std::runtime_error("Empty file: " + filename);

	GetTokens(content);

    while(!_tokens.empty())
        ParseServer();
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
		throw std::runtime_error("Error : Psrser::Syntax error!");

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

    config.address.port = DEFAULT_PORT;; 
	config.root.clear();
	config.allowedMethods.clear();
	config.serverName.clear();
	config.errorPages.clear();
	config.index.clear();
	config.clientMaxBodySize = -1;
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
		if (!isValidServerKey(token))
			throw std::runtime_error("Error : Parser::unknown  key " + token + "!");
		else if (token == "root")
			ParseRoot(server);
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

	//check if server name  and port match with the other server
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
		if (!isValidLocationKey(token))
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
		else if (token == "cgi_path")
			ParseCgiPath(location);
		else if (token == "root")
			ParseRoot(location);
		else if (token == "upload_path")
			ParseUploadPath(location);
	}

	// Add default method (GET) if no allowed method is specified
	if (location.allowedMethods.empty())
		location.allowedMethods.push_back("GET");

	// Check for required fields of locaiton context block?
	server.locations.push_back(location);
}


void Parse::ParseRoot(ServerContext& server)
{
    
	server.root = fullPath(ROOT, Accept());
	Skip(";");
}

void Parse::ParseRoot(LocationContext& location)
{
    
	location.root = fullPath(ROOT, Accept());
	Skip(";");
}

void Parse::ParseServerName(ServerContext& server)
{
	
    server.serverName = Accept();
	Skip(";");
}

void Parse::ParseAddress(ServerContext& server)
{
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

		int port = toInt(token);
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
    try
    {
		int value = toInt(Accept());
		if (value < 0)
			throw std::runtime_error("negative value");

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
			int code = toInt(tokens[i]);
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
	if (location.uri.empty() || location.uri[0] != '/')
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
	try
    {
		std::string token = Accept();
		std::string path = fullPath(ROOT, token);

		// Check if alias is accessible and is a directory
		struct stat pathInfo;
		if (stat(path.c_str(), &pathInfo) != 0 || !S_ISDIR(pathInfo.st_mode))
			throw std::runtime_error("Parser: invalid alias " + token + "!");


		location.alias = token; // path or alias?
		Skip(";");
	}
	catch (...)
    {
        /////
    }
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
	std::string token;
	while ((token = Accept()) != ";") 
		location.index.push_back(token);
}

void Parse::ParseRedirect(LocationContext& location)
{
	try 
    {
		location.redirect.first = toInt(Accept());
		location.redirect.second = Accept(); 
	}
	catch (std::exception& e) 
    {
		throw std::runtime_error("Parser: " + std::string(e.what()) + "!");
	}
}





void Parse::addDefaultLocation(ServerContext& server)
{
	// Check if the default location already exist
	for (std::vector<LocationContext>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
		if (it->uri == "/")
			return;

	LocationContext location = Location();
	location.uri = "/";
	location.allowedMethods.push_back("GET");
	location.index.push_back("index.html");

	server.locations.push_back(location);
}



void	Parse::ParseCgiPath(LocationContext& location)
{
	std::string path = Accept();
	location.cgiPath = path;
	Skip(";");
}


void Parse::ParseUploadPath(LocationContext &location)
{
	std::string path = Accept();

	struct stat pathInfo;
	if (stat(path.c_str(), &pathInfo) != 0 || !S_ISDIR(pathInfo.st_mode))
		throw std::runtime_error("Parser: invalid upload path " + path + "!");

	location.uploadPath = path;
	Skip(";");
}

