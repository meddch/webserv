#include "Client.hpp"

Client::Client(int fd, Listen_Addr Client, Listen_Addr Server)
{
	static int clientID = 1;
	_id = clientID;
	clientID++;

	_fd = fd;
	_serverAddr = Server;
	_clientAddr = Client;
	_ready = false;
	_Connected = true;

	_lastTime = std::time(NULL);
	cout << "Client " << _id << " bound to " << toIPString(_clientAddr.ip) << ":" << _clientAddr.port << endl;
	cout << "Client " << _id << " connected to " << toIPString(_serverAddr.ip) << ":" << _serverAddr.port << endl;
}

int Client::getId() const
{ 
	return _id;
}

int Client::getFd() const 
{
	return _fd;
}

int Client::getPort() const 
{
	return _serverAddr.port;
}

in_addr_t Client::getServerIp() const 
{
	return _serverAddr.ip;
}

in_addr_t Client::getClientIp() const 
{
	return _clientAddr.ip;
}

bool Client::is_Connected() const 
{
	return _Connected;
}

bool Client::Timeout() const 
{
	return (std::time(NULL) - _lastTime) > TIMEOUT_T; 
}


void Client::set_Connect(bool status)
{
	_Connected = status;
}

void Client::setReady(bool status)
{
	_ready = status;
}

bool Client::isReady() const
{
	return _ready;
}