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
	return it->second;
}

int Server::getMaxBodySize() const
{
    return _config.clientMaxBodySize;
}


bool Server::bodySizeAllowed(int bytes) const
{
	return getMaxBodySize() == -1 || getMaxBodySize() >= bytes;
}




int Server::getId() const
{
	return _id;
}


LocationContext Server::getLocation(std::string uri)
{
	std::vector<LocationContext>::iterator it;

	for (it = _config.locations.begin(); it != _config.locations.end(); it++)
		if (it->uri == uri)
			return *it;

	size_t backPos = uri.find_last_of('/');
	uri = backPos == 0 ? "/" : uri.substr(0, uri.find_last_of('/'));
	return getLocation(uri);
}



