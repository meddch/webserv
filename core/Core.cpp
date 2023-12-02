#include "Core.hpp"
#include "Server.hpp"
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/_types/_off_t.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/errno.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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
	(void)addr;

	if (bind(fd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0)
	{
		close(fd);
		throw std::runtime_error("Error: Bind failed: " + std::string(strerror(errno)));
	}

	if (listen(fd, 255))
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
		plfds.push_back(make_PlFd(fd, POLLIN));
		std::cout << "Server is running" << std:: endl;
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

			for (size_t i = 0; i < plfds.size(); i++)
			{
				if (i >= _nbr_sockets)
				{
					if (plfds[i].revents & POLLIN)
					{
						handlePl_IN(getClient(i));//request
					}
					else if (plfds[i].revents & POLLOUT)
						handlePl_Out(getClient(i));//re

				}
				else
				{
					if (plfds[i].revents & POLLIN)
					{
						Add_Client(plfds[i].fd);
						plfds[i].revents = 0;
					}
				}
				}
			ClearInvalidCnx();
		}
	}

	catch(std::exception &e)
	{
		puts("Error:");
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
		if (!client.is_Connected())
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
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	return pfd;
}

void Core::Add_Client(int listenFd)
{
	int clientFd = accept(listenFd, NULL, NULL);
	if (clientFd == -1)
		throw std::runtime_error("accept() failed: " + std::string(strerror(errno)));
	Client client(clientFd, getClientAddress(clientFd), getServerAddress(listenFd));
	_clients.insert(std::make_pair(clientFd, client));
	pollfd pfd = make_PlFd(clientFd, POLLIN);
	plfds.push_back(pfd);
	
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
	ssize_t buffer_size = 4097;

	try {
		char buffer[buffer_size];
		ssize_t bytesRead = recv(client.getFd(), buffer, buffer_size - 1, 0);
		if (bytesRead == -1 || bytesRead == 0)
		{
			client.set_Connect(false);
			return;
		}
		buffer[bytesRead] = '\0';
		//if connection true and timeout disconnect
		std::string Str(buffer, bytesRead);
		if (!client._requestParsed)
		{
			client.getREQ(Str);
			client.request._reqParsed = true;
			client.server = getServer(client);
			client._config_location = client.server.getLocation(client.request.getRequestURI());
			client.request.isMethodAllowed(client._config_location.allowedMethods);
		}
		if (!client._requestIsReady)
			client.getBody(Str);
		if (client._requestIsReady)
		{
			client.request.setRequestString(client._body);
			client.request.parseRequestBody();
			client.handleRequestMethod();
			client.setReady(true);
			setPlfdEvents(client.getFd(), POLLOUT);
		}
	}
	catch (const std::exception& e)
	{
		client.response.handleResponseError(client.request, (std::string)e.what());
		client.setReady(true);
		setPlfdEvents(client.getFd(), POLLOUT | POLLIN);
	}
}

void Core::handlePl_Out(Client& client)
{
	signal(SIGPIPE, SIG_IGN);
	if (client.isReady())
	{
		ssize_t bytesSent = 0;
		std::string Str;
		if (client.response.readyToSend)
		{
			Str = client.response.response;
			bytesSent = send(client.getFd(), Str.c_str(), Str.length(), 0);
			std::cout << Str << std::endl;
			client.set_Connect(false);

		}
		else if (client.request._headers.find("range") != client.request._headers.end()){
			struct stat filestat;
			stat(client.response.filePath.c_str(), &filestat);
			client.response._fileSize = filestat.st_size;
			std::stringstream ss;
			ss << client.request._headers["range"].substr(client.request._headers["range"].find("=") + 1);
			ss >> client.response._offset;
			client.response.generateChunkedResponse();
			std::fstream file;
			file.open(client.response.filePath.c_str());
			file.seekg(client.response._offset, std::ios::beg);
			Str = client.response.response;
			size_t len = Str.length();
			char Buffer[BYTES];
			file.read(Buffer, BYTES);
			ssize_t bytesRead = file.gcount();
			if (bytesRead == -1 || bytesRead == 0)
			{
				file.close();
				client.set_Connect(false);
				return;
			}
			Str += std::string(Buffer, bytesRead);
			len += bytesRead;
			bytesSent = send(client.getFd(), Str.c_str(), len, 0);
			file.close();
	
		}
		else if (client.request._headers.find("range") == client.request._headers.end())
		{
			if (!client.response._headerSent)
			{
				struct stat filestat;
				stat(client.response.filePath.c_str(), &filestat);
				client.response._contentLength = std::to_string(filestat.st_size);
				client.response.fd = open(client.response.filePath.c_str(), O_RDONLY);
				client.response.initResponseHeaders(client.request);
				Str = client.response.response;
				client.response._headerSent = true;
				bytesSent = send(client.getFd(), Str.c_str(), Str.length(), 0);
			}
			else
				{
					char buffer[BYTES];
					ssize_t bytesRead = read(client.response.fd, buffer, BYTES);
					if (bytesRead == -1 || bytesRead == 0)
					{
						close(client.response.fd);
						client.set_Connect(false);
						return;
					}
					bytesSent = send(client.getFd(), buffer, bytesRead, 0);
				}
			}
		}
}


Server&	Core::getServer(Client client)
{
	std::string Host;
	std::vector<Server> servers;
	int		id;
	
	if (client.request._headers.find("host") != client.request._headers.end())
		Host = client.request._headers["host"];
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
			if (servers[i].getName() == Host)
				id = servers[i].getId();
		}
		for (size_t i = 0; i < _servers.size(); i++)
		{
			if (_servers[i].getId() == id)
				return _servers[i];
		}
		Host = client.request._headers["host"];
	
		size_t colonPos = Host.find(':');
		in_addr_t ip = colonPos != std::string::npos ? toIpNum(Host.substr(0, colonPos)): toIpNum(Host);
		int port = colonPos != std::string::npos ? toIpNum(Host.substr(colonPos + 1)): 80;
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



void Core::setPlfdEvents(int fd, short events)
{
	for (size_t i = 0; i < plfds.size(); i++)
	{
		if (plfds[i].fd == fd)
		{
			plfds[i].events = events;
			return;
		}
	}
}
