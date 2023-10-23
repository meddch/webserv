#pragma once

#include "../parse/Utils.hpp"

class Server
{
private:
	ServerContext _config;
public:
	Server(const ServerContext config);


	Listen_Addr						getAddress() const;
	std::string            			getName() const;
	std::string             		getErrorPage(int code) const;
	int                				getMaxBodySize() const;
	bool               				bodySizeAllowed(int bytes) const;
	std::vector<std::string>		getMethods() const;
	std::vector<LocationContext>	getLocations() const;
	std::string						getRoot() const;
};