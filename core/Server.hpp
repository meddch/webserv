#pragma once

#include "../parse/utils.hpp"

class Server
{
private:
	ServerContext _config;
	int 			_id;
public:
	Server(const ServerContext config);
	Server();

	Listen_Addr						getAddress() const;
	std::string            			getName() const;
	std::string             		getErrorPage(int code) const;
	int                				getMaxBodySize() const;
	bool               				bodySizeAllowed(int bytes) const;
	std::vector<std::string>		getMethods() const;
	std::string						getRoot() const;
	int								getId() const;
	LocationContext 				&getLocation(std::string);


};