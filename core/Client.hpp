#pragma once
#include "../parse/Parse.hpp"
#include "Server.hpp"
#include "../request/Request.hpp"
#define TIMEOUT_T 40000
#define POST "POST"

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
	std::string	_httpBuffer;
	std::string	_chunkedBuffer;
	bool		_recvChunk;
	int 		_bytesRecved;
	int 		_bytesExpected;
public :
	Request		request;
	std::string	_body;
	bool 		_requestIsReady;
	bool 		_requestParsed;
	Client(int fd, Listen_Addr Client, Listen_Addr Server);
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
	void 		getREQ(std::string&);
	void 		reset();
	void 		getBody(std::string&);

};