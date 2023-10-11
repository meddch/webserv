#include "Core.hpp"

Core::Core(vector<ServerContext> configs)
{
    for (int i = 0; i < configs.size() ; i++)
        _servers.push_back(Server(configs[i]));
}