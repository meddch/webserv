#pragma once

#include "../parse/utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>



class Core
{
private :

	vector<struct pollfd>   plfds;
    map<int, Client>        _clients;
    vector<Server>          _servers;
	size_t					_nbr_sockets;
public :
    static	bool			runing;
    Core(vector<ServerContext> configs);
    int					CreateTcpIpListeners(Listen_Addr addr);
	set<Listen_Addr>    getUniqueAddresses(vector<Server>);
	void 				handlePl_IN(Client&);
	Client& 			getClient(int);
	void				init();
	void				Add_Client(int);
	void				run();
	void				ClearInvalidCnx();
	void 				Erase_PlFd(int);
	pollfd 				make_PlFd(int , short);
	Listen_Addr 		getServerAddress(int );	
	Listen_Addr 		getClientAddress(int );
	void 				handlePl_Out(Client& client);

};