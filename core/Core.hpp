#pragma once

#include "../parse/utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <poll.h>


class Core
{
private :

    vector<struct pollfd>   plfds;
    map<int, Client>        _clients;
    vector<Server>          _servers;
public :
    Core(vector<ServerContext> configs);
    void	CreateTcpIpListeners(set<Listen_Addr> s_addrs);
	set<Listen_Addr>    getUniqueAddresses(vector<Server> servers);
};