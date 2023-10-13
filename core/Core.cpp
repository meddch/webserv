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
	 while (1) 
	 {

	int conn;

	if ( (conn = accept(listener, NULL, NULL)) < 0 )
	    Error_Quit("Error calling accept()");


	/*  Fork child process to service connection  */

	if ( (pid = fork()) == 0 ) {

	    /*  This is now the forked child process, so
		close listening socket and service request   */

	    if ( close(listener) < 0 )
		Error_Quit("Error closing listening socket in child.");
	    
	    Service_Request(conn);


	    /*  Close connected socket and exit  */

	    if ( close(conn) < 0 )
		Error_Quit("Error closing connection socket.");
	    exit(EXIT_SUCCESS);
	}


	/*  If we get here, we are still in the parent process,
	    so close the connected socket, clean up child processes,
	    and go back to accept a new connection.                   */

	if ( close(conn) < 0 )
	    Error_Quit("Error closing connection socket in parent.");

	waitpid(-1, NULL, WNOHANG);
    }

}