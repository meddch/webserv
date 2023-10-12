#include "Server.hpp"


Server::Server(const ServerContext config) : _config(config)
{}

Listen_Addr Server::getAddress() const
{
    return _config.address;
}

string Server::getName() const
{ 
    return _config.serverName;
}

string Server::getErrorPage(int code) const
{
	map<int, string>::const_iterator it = _config.errorPages.find(code);
	return it == _config.errorPages.end() ? "" : fullPath(ROOT, it->second);
}

int Server::getMaxBodySize() const
{
    return _config.clientMaxBodySize;
}


bool Server::bodySizeAllowed(int bytes) const
{
	return getMaxBodySize() == -1 || getMaxBodySize() >= bytes;
}

