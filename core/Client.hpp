#pragma once
#include "../parse/ParseConfig.hpp"

class Client
{
private :
	int			_fd;
	bool		_Connected;
	Listen_Addr	_serverAddr;
	Listen_Addr	_clientAddr;
public :
	Client(int fd, Listen_Addr Client, Listen_Addr Server);
	Listen_Addr getClientAddress();
	Listen_Addr getServerAddress();
	int			getFd();
	bool		is_Connected();
	void		set_Connect(bool);
	bool		Timeout();
};