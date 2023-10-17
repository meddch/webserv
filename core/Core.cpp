#include "Core.hpp"

bool Core::runing = true;

Core::Core(vector<ServerContext> configs)
{
	_nbr_sockets = 0;
	for (size_t i = 0; i < configs.size() ; i++)
		_servers.push_back(Server(configs[i]));
}

// void	Core::CreateTcpIpListeners(void)
// {
// 	int fd;

// 	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
// 		throw runtime_error("socket() failed: " + string(strerror(errno)));

// 	int sockopt = 1;
// 	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, static_cast<const void *>(&sockopt), sizeof(int)) == -1) 
// 		throw runtime_error("setsockopt() failed: " + string(strerror(errno)));
	
// 	sockaddr_in serverAddr;
// 	memset(&serverAddr, 0, sizeof(serverAddr));
// 	serverAddr.sin_family = AF_INET;
// 	serverAddr.sin_port = htons(addr.port);
// 	serverAddr.sin_addr.s_addr = addr.ip;

// 	if (bind(fd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
// 		close(fd);
// 		throw runtime_error("bind() failed: " + string(strerror(errno)));
// 	}

// 	if (listen(fd, 10)) {
// 		throw runtime_error("listen() failed: " + string(strerror(errno)));
// 	}

// 	return ;

// }

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
			// else if (plfds[i].revents & POLLOUT)
				// handlePollOut(getClient(i));//reponse
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
		string Str(buffer, bytesRead);
		cout << Str;
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

