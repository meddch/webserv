#include "Utils.hpp"


// static helpers *******************************************

string ServerKeys[] = {"root", "server_name", "listen", "client_max_body_size", "error_page", "location"};

string LocationKeys[] = {"autoindex", "alias", "allowed_methods", "index", "return"};


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


bool Parse::isValidServerKey(string key)
{
	return std::find(validServerKeys.begin(), validServerKeys.end(), key) != validServerKeys.end();
}

bool Parse::isValidLocationKey(string key)
{
	return std::find(validLocationKeys.begin(), validLocationKeys.end(), key) != validLocationKeys.end();
}




//Constructor,Geters ******************************************

Parse::Parse(string const &filename)
{
    std::ifstream file(filename.c_str());
	if (file.fail())
		throw runtime_error("Failed to open file: " + filename);

    
	_config.clear();
    _tokens.clear();
	string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	if (content.empty())
		throw runtime_error("Empty file: " + filename);

	GetTokens(content);

    while(!_tokens.empty())
        ParseServer();
}

vector<ServerContext> Parse::GetConfig()
{
    return _config;
}


// Parsiing utils  >> *********************************************

void	Parse::GetTokens(const string& content)
{
    const string whitespaces(" \t\r\n"), spc_caratcter("{};");
	size_t pos = 0;

	while (pos < content.size())
	{
		if (spc_caratcter.find(content[pos]) != spc_caratcter.npos)
		{
			_tokens.push_back(string(1, content[pos]));
			pos++;
		}
		else if (whitespaces.find(content[pos]) != whitespaces.npos)
			pos++;
		else
		{
			size_t tokEnd = content.find_first_of(whitespaces + spc_caratcter, pos);
			if (tokEnd == string::npos)
				tokEnd = content.size();
			_tokens.push_back(content.substr(pos, tokEnd - pos));
			pos += _tokens.back().size();
		}
	}
}

string	Parse::Accept(void)
{
	if (_tokens.empty())
		throw runtime_error("Error : Psrser::Syntax error!");

	string token = _tokens.front();
	_tokens.pop_front();
	return token;
}

void	Parse::Skip(const string& token)
{
	if (_tokens.front() != token)
		throw runtime_error("Error : Parser::Can't find " + token + "!");

	_tokens.pop_front();
}

//*******************************************************************

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

	return config;
}

void	Parse::ParseServer()
{
	ServerContext server = Server();
	Skip("server"), Skip("{");

	string token;
	while ((token = Accept()) != "}")
	{
		if (isValidServerKey(token))
			throw runtime_error("Error : Parser::unknown  key " + token + "!");
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
	}

	//check if server name  and port match with the other server
	vector<ServerContext>::iterator it;
	for (it = _config.begin(); it != _config.end(); it++)
	{
		if (it->serverName == server.serverName && it->address.port == server.address.port)
			throw runtime_error("Parser: duplication server name " + it->serverName + "!");
	}
	
	addDefaultLocation(server);

	_config.push_back(server);
}

LocationContext Parse::Location(void)
{
	LocationContext location;

	// Set the default values
	location.alias = "";
	location.redirect.second = "";

	return location;
}

void Parse::ParseLocation(ServerContext& server)
{
	LocationContext location = Location();
	ParseUri(location), Skip("{");

	// Check if location uri already exists
	vector<LocationContext>::iterator it;
	for (it = server.locations.begin(); it != server.locations.end(); it++) 
		if (it->uri == location.uri)
			throw runtime_error("Parser: duplication location " + it->uri + "!");

	string token;
	while ((token = Accept()) != "}")
    {
		if (!isValidLocationKey(token))
			throw runtime_error("Error : Parser::Unknown key " + token + "!");
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
	}

	// Add default method (GET) if no allowed method is specified
	if (location.allowedMethods.empty())
		location.allowedMethods.push_back("GET");

	// Check for required fields of locaiton context block?
	// if 
	server.locations.push_back(location);
}


void Parse::ParseRoot(ServerContext& server)
{
    server.root = fullPath(ROOT, Accept());
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
		string token = Accept();
		size_t colonPos = token.find(":");
		
		if (colonPos != string::npos)
        {
			server.address.ip = toIPv4(token.substr(0, colonPos));
			token.erase(token.begin(), token.begin() + colonPos + 1);
		}
		else 
			server.address.ip = 0;

		int port = toInt(token);
		if (port <= 0 || port > 65535)
			throw runtime_error("Parser : Invalid Port!");
		server.address.port = port;

		Skip(";"); 
	}
	catch (...)
    {
		throw runtime_error("Parser: invalid listen!");
	}
}

void Parse::ParseClientMaxBodySize(ServerContext& server)
{
    try
    {
		int value = toInt(Accept());
		if (value < 0)
			throw runtime_error("negative value");

		server.clientMaxBodySize = value;
		Skip(";");
	}
	catch (exception& e)
    {
		throw runtime_error("Parser: body size, " + string(e.what()) + "!");
	}
}


void Parse::ParseErrorPage(ServerContext& server)
{
    vector<string> tokens;
	string token;

	while ((token = Accept()) != ";")
        tokens.push_back(token);

	if (tokens.size() < 2) 
		throw runtime_error("Parser: syntax error!");

	try {
		for (size_t i = 0; i < tokens.size() - 1; i++)
        {
			int code = toInt(tokens[i]);
			map<int, string>::iterator it;
			if (server.errorPages.find(code) != server.errorPages.end())
				continue ;
			server.errorPages.insert(make_pair(code, tokens.back()));
		}
	}
	catch (exception& e)
    {
		throw runtime_error("Parser: error page, " + string(e.what()) + "!");
	}
}


void Parse::ParseUri(LocationContext& location)
{
	
    location.uri = Accept();
	// validate, it has to start with '/'?
	// any illegal characters in the uri?
}

void Parse::ParseAutoindex(LocationContext& location)
{
	string token = Accept();
	if (token != "on" && token != "off")
		throw runtime_error("Parser: invalid autoindex value!");

	location.autoindex = token == "on" ? true : false;
	Skip(";");
}

void Parse::ParseAlias(LocationContext& location)
{
	try
    {
		string token = Accept();
		string path = fullPath(ROOT, token);

		// Check if alias is accessible and is a directory
		struct stat pathInfo;
		if (stat(path.c_str(), &pathInfo) != 0 || !S_ISDIR(pathInfo.st_mode))
			throw runtime_error("Parser: invalid alias " + token + "!");


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
	string token;
	while ((token = Accept()) != ";")
    {
        // Define all allowed methods somewhere else in an array?
		// Potentially more and need to be accessible in other places
		if (token != "GET" && token != "POST" && token != "DELETE" ) 
			throw runtime_error("Parser: unknown method " + token);

		location.allowedMethods.push_back(token);
	}
}

void Parse::ParseIndex(LocationContext& location)
{
	string token;
	while ((token = Accept()) != ";") 
		location.index.push_back(token);
}

void Parse::ParseRedirect(LocationContext& location)
{
	try 
    {
		location.redirect.first = toInt(Accept());
		location.redirect.second = Accept(); 
		Skip(";");
	}
	catch (exception& e) 
    {
		throw runtime_error("Parser: " + string(e.what()) + "!");
	}
}





void Parse::addDefaultLocation(ServerContext& server)
{
	// Check if the default location already exist
	for (vector<LocationContext>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
		if (it->uri == "/")
			return;

	LocationContext location = Location();
	location.uri = "/";
	location.allowedMethods.push_back("GET");
	location.index.push_back("index.html");

	server.locations.push_back(location);
}


void	Parse::ParseCgi(LocationContext& location)
{
	string token = Accept();
	if (token != "on" && token != "off")
		throw runtime_error("Parser: invalid cgi value!");

	location.cgi = token == "on" ? true : false;
	Skip(";");
}

void	Parse::ParseCgiPath(LocationContext& location)
{
	string token = Accept();
	string path = fullPath(ROOT, token);

	// Check if cgi path is accessible and is a directory
	struct stat pathInfo;
	if (stat(path.c_str(), &pathInfo) != 0 || !S_ISDIR(pathInfo.st_mode))
		throw runtime_error("Parser: invalid cgi path " + token + "!");

	location.cgiPath = token;
	Skip(";");
}

void	Parse::ParseCgiExtension(LocationContext& location)
{
	string token = Accept();
	if (token[0] != '.')
		throw runtime_error("Parser: invalid cgi extension " + token + "!");

	location.cgiExtension = token;
	Skip(";");
}


