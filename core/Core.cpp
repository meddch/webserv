#include "Core.hpp"

Core::Core(vector<ServerContext> configs)
{
	for (int i = 0; i < configs.size() ; i++)
		_servers.push_back(Server(configs[i]));
}

void	Core::CreateTcpIpListeners(set<Listen_Addr> s_addrs)
{
	set<Listen_Addr> uniques = getUniqueAddresses(_servers);
	int fd;

	for (set<Listen_Addr>::iterator it = uniques.begin(); it != uniques.end(); it++)
	{

    	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
			printf("Couldn't create listening socket.\n");


    	struct sockaddr_in server_addr;
    	memset(&server_addr, 0, sizeof(server_addr));
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_port = htons((*it).port);
    	server_addr.sin_addr.s_addr = (*it).ip;

   		if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
   	    printf("Bind ERR\n");

		if(listen(fd , 100)) //how many ?
			printf("Listen ERR\n");

		struct pollfd pfd;
		pfd.fd = fd;
		pfd.events = POLLIN;

		plfds.push_back(pfd);
    }
}

set<Listen_Addr> Core::getUniqueAddresses(vector<Server> servers)
{

	set<Listen_Addr> uniques;
	for (size_t i = 0; i < servers.size(); i++)
    	uniques.insert(servers[i].getAddress());
	return uniques;
}

void	Core::run()
{
	

}