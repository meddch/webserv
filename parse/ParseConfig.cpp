#include "utils.hpp"

const string ServerKeys[] = {"root", "server_name",  "listen", "client_max_body_size", "error_page", "location"};

const string Locationkeys[] = {"autoindex", "alias", "allowed_method", "index", "return"};


const vector<string> ParseConfig::validServerKeys(ServerKeys, ServerKeys + sizeof(ServerKeys) / sizeof(ServerKeys[0]));

const vector<string> ParseConfig::validLocationKeys(Locationkeys, Locationkeys + sizeof(Locationkeys) / sizeof(Locationkeys[0]));


bool ParseConfig::isValidServerKey(string key)
{
	return std::find(validServerKeys.begin(), validServerKeys.end(), key) != validServerKeys.end();
}

bool ParseConfig::isValidLocationKey(string key)
{
	return std::find(validLocationKeys.begin(), validLocationKeys.end(), key) != validLocationKeys.end();
}





ParseConfig::ParseConfig(string const &filename)
{
    std::ifstream file(filename.c_str());
	if (file.fail())
		throw runtime_error("Failed to open file: " + filename);
    
	_config.clear();
    _tokens.clear();
	string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	GetTokens(content);

    while(!_tokens.empty())
        ParseServer();
}

vector<ServerContext> ParseConfig::GetConfig()
{
    return _config;
}


//Get all tokens
void	ParseConfig::GetTokens(const string& content)
{
    const string whitespaces(WHITESPACES), spc_caratcter("{};");
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

string	ParseConfig::Accept(void)
{
	if (_tokens.empty())
		throw runtime_error("Parser: syntax error!");

	string token = _tokens.front();
	_tokens.pop_front();
	return token;
}

void	ParseConfig::Skip(const string& token)
{
	if (_tokens.front() != token)
		throw runtime_error("can't find " + token + "!");

	_tokens.pop_front();
}


ServerContext ParseConfig::CreateServer(void)
{
	ServerContext config;

	memset(&config, 0, sizeof(ServerContext));
	config.root = ROOT;
	config.clientMaxBodySize = -1;

	return config;
}

void	ParseConfig::ParseServer()
{
	ServerContext server = CreateServer();
	Skip("server"), Skip("{");

	string token;
	while ((token = Accept()) != "}")
	{
		if (!isValidServerKey(token))
			throw runtime_error("Parser: unknown server key " + token + "!");
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

	if (!server.address.ip && !server.address.port)
		throw runtime_error("Parser: server has no listen field!");

	// Fill in the default error pages and '/' location if not provided
	addDefaultErrorPages(server);
	addDefaultLocation(server);

	_config.push_back(server);
}


void ParseConfig::ParseLocation(ServerContext& server)
{
	LocationContext location = CreateLocation();
	ParseUri(location), Skip("{");

	// Check if location uri already exists
	vector<LocationContext>::iterator it;
	for (it = server.locations.begin(); it != server.locations.end(); it++) 
    {
		if (it->uri == location.uri)
			throw runtime_error("Parser: duplication location " + it->uri + "!");
	}

	string token;
	while ((token = Accept()) != "}")
    {
		if (!isValidLocationKey(token))
			throw runtime_error("Parser: unknown location key " + token + "!");
		else if (token == "autoindex")
			ParseAutoindex(location);
		else if (token == "alias")
			ParseAlias(location);
		else if (token == "allowed_method")
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

	server.locations.push_back(location);
}


void ParseConfig::ParseRoot(ServerContext& server)
{
	server.root = fullPath(ROOT, Accept());
	Skip(";");
}

void ParseConfig::ParseServerName(ServerContext& server)
{
	server.serverName = Accept();
	Skip(";");
}

void ParseConfig::ParseAddress(ServerContext& server)
{
	try
    {
		string token = Accept();

		// Resolve host portion
		size_t colonPos = token.find(":");
		if (colonPos != string::npos)
        {
			server.address.ip = toIPv4(token.substr(0, colonPos));
			token.erase(token.begin(), token.begin() + colonPos + 1);
		}
		else 
			server.address.ip = 0;

		// Resolve port portion
		int port = toInt(token);
		if (port <= 0 || port > 65535)
			throw exception();
		server.address.port = port;

		Skip(";"); 
	}
	catch (...)
    {
		throw runtime_error("Parser: invalid listen!");
	}
}

void ParseConfig::ParseClientMaxBodySize(ServerContext& server)
{
	try
    {
		int value = toInt(Accept());
		if (value < 0)
			throw runtime_error("negative value");

		server.clientMaxBodySize = value;
		Skip(";");
	}
	catch (exception& e) {
		throw runtime_error("Parser: body size, " + string(e.what()) + "!");
	}
}


void ParseConfig::ParseErrorPage(ServerContext& server)
{
	vector<string> tokens;
	string token;

	while ((token = Accept()) != ";")
        tokens.push_back(token);

	if (tokens.size() < 2) 
		throw runtime_error("Parser: syntax error!");

	// Excluding the last element which is the page path
	try {
		for (size_t i = 0; i < tokens.size() - 1; i++)
        {
			int code = toInt(tokens[i]);

			map<int, string>::iterator it;
			if (server.errorPages.find(code) != server.errorPages.end())
				continue ;


			server.errorPages[code] = tokens.back();
		}
	}
	catch (exception& e)
    {
		throw runtime_error("Parser: error page, " + string(e.what()) + "!");
	}
}


void ParseConfig::ParseUri(LocationContext& location)
{
	location.uri = Accept();
	// validate, it has to start with '/'?
	// any illegal characters in the uri?
}

void ParseConfig::ParseAutoindex(LocationContext& location)
{
	string token = Accept();
	if (token != "on" && token != "off")
		throw runtime_error("Parser: invalid autoindex value!");

	location.autoindex = token == "on" ? true : false;
	Skip(";");
}

void ParseConfig::ParseAlias(LocationContext& location)
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

void ParseConfig::ParseAllowedMethods(LocationContext& location)
{
	string token;
	while ((token = Accept()) != ";")
    {
		// Define all allowed methods somewhere else in an array?
		// Potentially more and need to be accessible in other places
		if (token != "GET" && token != "POST" && token != "DELETE") 
			throw runtime_error("Parser: unknown method " + token);

		location.allowedMethods.push_back(token);
	}
}

void ParseConfig::ParseIndex(LocationContext& location)
{
	string token;
	while ((token = Accept()) != ";") 
		location.index.push_back(token);
}

void ParseConfig::ParseRedirect(LocationContext& location)
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



LocationContext ParseConfig::CreateLocation(void)
{
	LocationContext location;

	// Set the default values
    memset(&location, 0, sizeof(LocationContext));
	location.alias = "";
	location.redirect.second = "";

	return location;
}


void ParseConfig::addDefaultErrorPages(ServerContext& server)
{
	for (size_t i = 0; i < validErrorCodes.size(); i++)
    {
		int code = validErrorCodes[i];
		if (server.errorPages.find(code) == server.errorPages.end())
			server.errorPages[code] = "/default_error/" + toString(code) + ".html";
	}
}


void ParseConfig::addDefaultLocation(ServerContext& server)
{
	// Check if the default location already exist
	for (vector<LocationContext>::iterator it = server.locations.begin(); it != server.locations.end(); it++)
		if (it->uri == "/")
			return;

	LocationContext location = CreateLocation();
	location.uri = "/";
	location.allowedMethods.push_back("GET");
	location.index.push_back("index.html");

	server.locations.push_back(location);
}
