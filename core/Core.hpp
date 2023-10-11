#pragma once

#include "../parse/utils.hpp"
#include "Client.hpp"
#include "Server.hpp"

class Core
{
private :

    vector<struct pollfd> plfds;
    map<int, Client> map_clients;
    vector<Server> _servers;
public :
    Core(vector<ServerContext> configs);
};