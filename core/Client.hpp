#pragma once
#include "../parse/Parse.hpp"
#include "Server.hpp"
#define TIMEOUT_T 40000

class Client
{
private :
	int			_fd;
	int			_id;
	bool		_Connected;
	Listen_Addr	_serverAddr;
	Listen_Addr	_clientAddr;
	time_t		_lastTime;
	bool		_ready;
public :
	Client(int fd, Listen_Addr Client, Listen_Addr Server);
	Listen_Addr getClientAddress() const;
	Listen_Addr getServerAddress() const;
	in_addr_t 	getServerIp() const;
	in_addr_t 	getClientIp() const;
	int 		getPort() const ;
	int			getFd() const;
	int			getId() const;
	bool		is_Connected() const;
	void		set_Connect(bool);
	bool		Timeout() const;
	void		setReady(bool);
	bool		isReady() const;
};