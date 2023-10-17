#include "Core.hpp"

Core::Core(vector<ServerContext> configs)
{
	runing = true;
	for (int i = 0; i < configs.size() ; i++)
		_servers.push_back(Server(configs[i]));
}

void	Core::CreateTcpIpListeners(void)
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

		plfds.push_back(make_PlFd(fd, POLLIN | POLLOUT));
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
	try 
	{
		CreateTcpIpListeners();
		while (runing)
		{
		int pollReady = poll(plfds.data(), plfds.size(), 1000);
		if (pollReady == -1)
			throw runtime_error("poll() failed");

		for (size_t i = _nbr_sockets; i < plfds.size(); i++)
		{
			if (plfds[i].revents & POLLIN)
				handlePollIN(getClient(i));//request
			else if (plfds[i].revents & POLLOUT)
				handlePollOut(getClient(i));//reponse
		}

		for (size_t i = 0; i < _nbr_sockets; i++)
			if (plfds[i].revents & POLLIN)
				Add_Client(plfds[i].fd);
		}
		ClearInvalidCnx();
	}
	catch(exception &e)
	{
		cerr << e.what();
	}
}

void Core::handlePollIN(Client& client)
{
	ssize_t buffer_size = 100000;

	char buffer[buffer_size];// buffer size?
	ssize_t bytesRead = recv(client.getFd(), buffer, buffer_size, 0);

	if (bytesRead == -1 || bytesRead == 0)
	{
		client.set_Connect(true);
		return;
	}

	try {
		string bufferStr(buffer, bytesRead);
		client.handlerequest(bufferStr);
		/// parse request and get response
		}
	catch (const exception& e)
	{
		cerr << e.what();
		// send err response 
	}
}

Client& Core::getClient(int key)
{
	map<int, Client>::iterator it;
	it = _clients.find(plfds[key].fd);
	Client& client = it->second;
	return client;
}

void Core::ClearInvalidCnx(void)
{
	map<int, Client>::iterator it;
	vector<map<int, Client>::iterator> removeIterators;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		Client client = it->second;
		if (client.is_Connected() || client.Timeout())
		{
			removeIterators.push_back(it);
			Erase_PlFd(client.getFd());
			close(client.getFd());
		}
	}

	for (size_t i = 0; i < removeIterators.size(); i++)
		_clients.erase(removeIterators[i]);

	removeIterators.clear();
}


void Core::Erase_PlFd(int fd)
{
	for (size_t i = 0; i < plfds.size(); i++)
	{
		if (plfds[i].fd == fd)
		{
			plfds.erase(plfds.begin() + i);
			return;
		}
	}
}

pollfd Core::make_PlFd(int fd, short events)
{
	pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	return pfd;
}

void Core::Add_Client(int listenFd)
{
	int clientFd = accept(listenFd, NULL, NULL);

	Client client(clientFd, getClientAddress(clientFd), getServerAddress(listenFd));
	_clients.insert(std::make_pair(clientFd, client));

	plfds.push_back(make_PlFd(clientFd, POLLIN | POLLOUT));
}

Listen_Addr Core::getServerAddress(int fd)
{
	sockaddr_in serverAddress;
	socklen_t addrLen = sizeof(serverAddress);
	if (getsockname(fd, reinterpret_cast<sockaddr*>(&serverAddress), &addrLen) == -1)
		throw runtime_error("getsockname() failed: " + string(strerror(errno)));

	Listen_Addr addr;
	addr.ip = serverAddress.sin_addr.s_addr;
	addr.port = ntohs(serverAddress.sin_port);

	return addr;
}


Listen_Addr Core::getClientAddress(int fd)
{
	sockaddr_in clientAddress;
	socklen_t addrLen = sizeof(clientAddress);
	if (getpeername(fd, reinterpret_cast<sockaddr*>(&clientAddress), &addrLen) == -1)
		throw runtime_error("getpeername() failed: " + string(strerror(errno)));

	Listen_Addr addr;
	addr.ip = clientAddress.sin_addr.s_addr;
	addr.port = ntohs(clientAddress.sin_port);

	return addr;
}