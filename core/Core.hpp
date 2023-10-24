#pragma once

#include "../parse/Utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../request/Request.hpp"



class Core
{
private :

	std::vector<struct pollfd>   		plfds;
   	std::map<int, Client>       		_clients;
    std::vector<Server>         		_servers;
	size_t								_nbr_sockets;
    static	bool						runing;
public :
    Core(std::vector<ServerContext> configs);

	static bool 			isRuning();
    int						CreateTcpIpListeners(Listen_Addr addr);
	std::set<Listen_Addr>   getUniqueAddresses();
	void 					handlePl_IN(Client&);
	Client& 				getClient(int);
	void					init();
	void					Add_Client(int);
	void					run();
	void					ClearInvalidCnx();
	void 					Erase_PlFd(int);
	pollfd 					make_PlFd(int , short);
	Listen_Addr 			getServerAddress(int );	
	Listen_Addr 			getClientAddress(int );
	void 					handlePl_Out(Client& client);

};