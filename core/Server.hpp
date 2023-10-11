#pragma once

#include "../parse/utils.hpp"

class Server
{
private:
	ServerContext _config;
public:
	Server(const ServerContext config);


	Listen_Addr			getAddress() const;
	string              getName() const;
	string              getErrorPage(int code) const;
	int                 getMaxBodySize() const;
	bool                bodySizeAllowed(int bytes) const;
    set<Listen_Addr>    getUniqueAddresses(vector<Server> servers);

};