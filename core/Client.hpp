#pragma once
#include "../parse/Parse.hpp"
#include "Server.hpp"
#include "../request/Request.hpp"
#include "../response/response.hpp"
#include "MACS.hpp"


class Client
{
	public :
	std::string		_buffer;
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
	Server		_server;
public :
	Request		request;
	Server 		server;
	Response	response;
	std::string	_body;
	bool 		_requestIsReady;
	bool 		_requestParsed;
	LocationContext	_config_location;
	Client(int fd, Listen_Addr Client, Listen_Addr Server);
	in_addr_t 	getServerIp() const;
	in_addr_t 	getClientIp() const;
	int 		getServerPort() const ;
	int			getFd() const;
	int			getId() const;
	bool		is_Connected() const;
	void		set_Connect(bool);
	bool		Timeout() const;
	void		setReady(bool);
	bool		isReady() const;
	void 		getREQ(std::string);
	void 		reset();
	void 		getBody(std::string);
	void		setServer(Server&);
	void		matchLocation(std::vector<LocationContext>);
	void		createUploadFile(std::string filename, std::string content);
	void 		handleRequestMethod();
	bool 		isMethodAllowed();
	void 		getLocations(Server&);
	void 		handleGetRequest();
	void 		handlePostRequest();
	void 		handleDeleteRequest();

};
