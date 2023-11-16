#include "Core.hpp"

bool Core::runing = true;

Core::Core(std::vector<ServerContext> configs)
{
	_nbr_sockets = 0;
	for (size_t i = 0; i < configs.size() ; i++)
		_servers.push_back(Server(configs[i]));
}

int	Core::CreateTcpIpListeners(Listen_Addr addr)
{
	int fd;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		throw std::runtime_error("socket() failed: " + std::string(strerror(errno)));

	int sock_opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, static_cast<const void *>(&sock_opt), sizeof(int)) == -1) 
		throw std::runtime_error("setsockopt() failed: " + std::string(strerror(errno)));
	
	sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(addr.port);
	serverAddr.sin_addr.s_addr = addr.ip; 

	if (bind(fd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
		close(fd);
		throw std::runtime_error("Error: Bind failed: " + std::string(strerror(errno)));
	}

	if (listen(fd, 10))
		throw std::runtime_error("Error: Listen failed: " + std::string(strerror(errno)));
	
	return fd;
}

std::set<Listen_Addr> Core::getUniqueAddresses()
{

	std::set<Listen_Addr> unique;
	for (size_t i = 0; i < _servers.size(); i++)
    	unique.insert(_servers[i].getAddress());
	return unique;
}

void	Core::init()
{
	std::set<Listen_Addr> uniques = getUniqueAddresses();
	std::set<Listen_Addr>::iterator it;
	for (it = uniques.begin(); it != uniques.end(); it++)
	{
		int fd = CreateTcpIpListeners(*it);
		_nbr_sockets++;
		plfds.push_back(make_PlFd(fd, POLLIN | POLLOUT));
		std::cout << "Server is running" <<std:: endl;
		std::cout << "Listening on " << toIPString(it->ip) << ":" << it->port << std::endl;
	}
}


void	Core::run()
{
	try 
	{
		init();
		while (runing)
		{
			int pollReady = poll(plfds.data(), plfds.size(), 1000);
			if (pollReady == -1)
				throw std::runtime_error("poll() failed");

			for (size_t i = _nbr_sockets; i < plfds.size(); i++)
			{
				if (plfds[i].revents & POLLIN)
					handlePl_IN(getClient(i));//request
				else if (plfds[i].revents & POLLOUT)
					handlePl_Out(getClient(i));//reponse
			}

			for (size_t i = 0; i < _nbr_sockets; i++)
				if (plfds[i].revents & POLLIN)
					Add_Client(plfds[i].fd);
			ClearInvalidCnx();
		}
	}

	catch(std::exception &e)
	{
		std::cerr << e.what();
	}
}


Client& Core::getClient(int key)
{
	std::map<int, Client>::iterator it;
	it = _clients.find(plfds[key].fd);
	Client& client = it->second;
	return client;
}

void Core::ClearInvalidCnx(void)
{
	std::map<int, Client>::iterator it;
	std::vector<std::map<int, Client>::iterator> removeIterators;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		Client client = it->second;
		if (!client.is_Connected() || client.Timeout())
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
		throw std::runtime_error("getsockname() failed: " + std::string(strerror(errno)));

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
		throw std::runtime_error("getpeername() failed: " + std::string(strerror(errno)));

	Listen_Addr addr;
	addr.ip = clientAddress.sin_addr.s_addr;
	addr.port = ntohs(clientAddress.sin_port);

	return addr;
}

void Core::handlePl_IN(Client& client)
{
	ssize_t buffer_size = 100000;

	char buffer[buffer_size];// buffer size?
	ssize_t bytesRead = recv(client.getFd(), buffer, buffer_size, 0);

	if (bytesRead == -1 || bytesRead == 0)
	{
		std::cout << "Client " << client.getId() << " disconnected" << std::endl;
		client.set_Connect(false);
		return;
	}

	try {
		//if connection true and timeout disconnect
		std::string Str(buffer, bytesRead);
		if (!client._requestParsed)
			client.getREQ(Str);
		if (!client._requestIsReady)
			client.getBody(Str);
		if (client._requestIsReady)
		{
			client.request.setRequestString(client._body);
			client.request.parseRequestBody();
			client.setReady(true);
			client.setServer(getServer(client));
			client.handleRequestMethod();
			client.response.generateResponse(client.request);
			client.request.toString();
		}
		// client.reset();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
		// send err response 
	}
}

void Core::handlePl_Out(Client& client)
{

	ssize_t bytesSent = 0;
	// std::string Str = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 20\r\n\r\n<h1>Hello World<h1/>";
	//if a request is ready to be sent
	//notify the client
	//send the response
	if (!client.is_Connected())
			return;
	if (client.isReady())
	{
	std::string Str = client.response.generateResponse(client.request);

		bytesSent = send(client.getFd(), Str.c_str(), Str.length(), 0);
		if (bytesSent == -1 || bytesSent == 0)
		{
			client.set_Connect(false);
			std::cout << "Client " << client.getId() << " disconnected" << std::endl;
			client.setReady(false);
			close(client.getFd());
				return;
		}
		client.setReady(false);
	}
}

Server&	Core::getServer(Client client)
{
	std::string Host;
	std::vector<Server> servers;
	int		id;
	
	if (client.request._headers.find("Host") != client.request._headers.end())
		Host = client.request._headers["Host"];
	else
		std::runtime_error("Error: No Host found");

	try {
		if (Host.find(":") != std::string::npos)
			Host = Host.substr(0, Host.find(":"));
		for (size_t i = 0; i < _servers.size(); i++)
		{
			if (_servers[i].getAddress().port == client.getServerPort())
				servers.push_back(_servers[i]);
		}
		if (servers.size() == 0)
			throw std::runtime_error("Error: No server found");
		id = -1;
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::cout << servers[i].getName() << std::endl;
			if (servers[i].getName() == Host)
				id = servers[i].getId();
		}
		for (size_t i = 0; i < _servers.size(); i++)
		{
			if (_servers[i].getId() == id)
				return _servers[i];
		}
		Host = client.request._headers["Host"];
	
		size_t colonPos = Host.find(':');
		in_addr_t ip = colonPos != std::string::npos ? toIpNum(Host.substr(0, colonPos)): toIpNum(Host);
		int port = colonPos != std::string::npos ? toInt(Host.substr(colonPos + 1)): 80;
		if (port <= 0 || port > 65535)
			throw std::runtime_error("port out of range");
		for (size_t i = 0; i < _servers.size(); i++)
		{
			Listen_Addr addr = _servers[i].getAddress();
			if (addr.ip == ip && addr.port == port)
				return _servers[i];
		}

	}
	catch (...) {
	}
	return _servers[0];
}

