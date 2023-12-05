#include "Core.hpp"
#include "Server.hpp"
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include "../Webserv.hpp"


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
		while (_running)
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
		clear_All();
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
		
		std::string Str(buffer, bytesRead);
		if (!client._requestParsed)
		{
			client.getREQ(Str);
			client.request._reqParsed = true;
			client.server = getServer(client);
			client._config_location = client.server.getLocation(client.request.getRequestURI());
			client.request.isMethodAllowed(client._config_location.allowedMethods);
			if (client._config_location.redirect.first)
				throw std::runtime_error("301");
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
		std::string code = e.what();
		if (code == REDIRECT){
			client.generateRedirectionResponse(client.request, client._config_location.redirect.second, client._config_location.redirect.first);
			std::cout << client.response.response << std::endl;
		}
		else
			client.response.handleResponseError(client.request, client.server.getErrorPage(std::atoi(code.c_str())) ,code);
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
			if (bytesSent == -1 || bytesSent == 0)
			{
				client.set_Connect(false);
				return;
			}
			if (client._isCGI)
				client.set_Connect(false);
		}
		else
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
					char buffer[10240];
					ssize_t bytesRead = read(client.response.fd, buffer, 10240);
					if (bytesRead == -1 || bytesRead == 0 )
					{
						close(client.response.fd);
						client.set_Connect(false);
						return;
					}
					bytesSent = send(client.getFd(), buffer, bytesRead, 0);
					if (bytesSent == -1 || bytesSent == 0 || bytesSent < 10240)
					{
						close(client.response.fd);
						client.set_Connect(false);
						return;
					}
				}
			}
		}
}


Server&	Core::getServer(Client client)
{
	
try {
		for (size_t i = 0; i < _servers.size(); i++)
		{
			if (_servers[i].getAddress().port == client.getServerPort())
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


void Core::clear_All(void)
{
	for (size_t i = 0; i < plfds.size(); i++)
		close(plfds[i].fd);
	_clients.clear();
	plfds.clear();
	_servers.clear();
}