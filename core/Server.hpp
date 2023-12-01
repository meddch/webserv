#pragma once

#include "../parse/utils.hpp"
#include <string>

class Server
{
private:
	int 			_id;
public:
	ServerContext _config;
	Server(const ServerContext config);
	Server();

	Listen_Addr						getAddress() const;
	std::string            			getName() const;
	std::string             		getErrorPage(int code) const;
	ssize_t                			getMaxBodySize() const;
	bool               				bodySizeAllowed(int bytes) const;
	std::vector<std::string>		getMethods() const;
	std::string						getRoot() const;
	int								getId() const;
	LocationContext 				&getLocation(std::string);
	bool 							uploadEnabled() const;
	// void							handlerequest(Request &request, Client &client);

};