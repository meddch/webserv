#include "Server.hpp"

Server::Server()
{}

Server::Server(const ServerContext config) : _config(config)
{
	std::cout << "Server " << _config.serverName << " created" << std::endl;
	static int serverID = 1;
	_id = serverID;
	serverID++;
}

Listen_Addr Server::getAddress() const
{
    return _config.address;
}

std::string Server::getName() const
{ 
    return _config.serverName;
}

std::string Server::getRoot() const
{
	return _config.root;
}

std::string Server::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = _config.errorPages.find(code);
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


std::vector<LocationContext> Server::getLocations() const
{
	return _config.locations;
}

std::vector<std::string> Server::getMethods() const
{
	return _config.allowedMethods;
}

int Server::getId() const
{
	return _id;
}









