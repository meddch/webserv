#include "Server.hpp"
#include <iostream>

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
	if (it == _config.errorPages.end())
		return EMPTY;
	return getRoot() + it->second;
}

ssize_t Server::getMaxBodySize() const
{
    return _config.clientMaxBodySize;
}


bool Server::bodySizeAllowed(int bytes) const
{
	return getMaxBodySize() == -1 || getMaxBodySize() >= bytes;
}

bool Server::uploadEnabled() const
{
	return _config.upload;
}


int Server::getId() const
{
	return _id;
}


LocationContext &Server::getLocation(std::string uri)
{
	std::vector<LocationContext>::iterator it;

	if (uri.find(".") != std::string::npos)
	{
		std::string extension = uri.substr(uri.find_last_of("."));
		for (it = _config.locations.begin(); it != _config.locations.end(); it++)
		{
			if (it->uri == extension)
				return *it;
		}
	}
	return findLocation(uri);
}
LocationContext &Server::findLocation(std::string uri)
{
	std::vector<LocationContext>::iterator it;

	for (it = _config.locations.begin(); it != _config.locations.end(); it++)
	{
		if (it->uri == uri)
			return *it;
	}

	size_t backPos = uri.find_last_of('/');
	uri = backPos == 0 ? "/" : uri.substr(0, uri.find_last_of('/'));
	return findLocation(uri);
}

void Server::addSession(std::string session_id, std::string username)
{
	_sessions[session_id] = username;
}

void Server::removeSession(std::string session_id)
{
	_sessions.erase(session_id);
}

bool Server::isSessionValid(std::string session_id)
{
	return _sessions.find(session_id) != _sessions.end();
}

std::string Server::getSessionUsername(std::string session_id)
{
	return _sessions[session_id];
}