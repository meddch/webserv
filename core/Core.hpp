#pragma once

#include "../parse/utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <poll.h>
#include <netinet/in.h>

extern bool runing;

class Core
{
private :

    vector<struct pollfd>   plfds;
    map<int, Client>        _clients;
    vector<Server>          _servers;
	ssize_t					_nbr_sockets;
public :
    Core(vector<ServerContext> configs);
    void				CreateTcpIpListeners();
	set<Listen_Addr>    getUniqueAddresses(vector<Server>);
	void 				handlePollIN(Client&);
	Client& 			getClient(int);
	void				Add_Client(int);
	void				run();
	void				ClearInvalidCnx();
	void 				Erase_PlFd(int);
	pollfd 				make_PlFd(int , short);
	Listen_Addr 		getServerAddress(int );	
	Listen_Addr 		getClientAddress(int );

};