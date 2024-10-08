#pragma once
#include "../parse/Parse.hpp"
#include "Server.hpp"
#include "../request/Request.hpp"
#include "../response/Response.hpp"
#include "../parse/Utils.hpp"
#include "MACS.hpp"
#include <ctime>
#include <fstream>
#include <sys/_types/_size_t.h>


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
	std::clock_t		_lastTime;
	bool		_ready;
	std::string	_httpBuffer;
	std::string	_chunkedBuffer;
	bool		_recvChunk;
	int 		_bytesExpected;
	Server		_server;
public :
	int 		_bytesRecved;
	Request		request;
	Server 		server;
	Response	response;
	std::string	_body;
	bool 		_requestIsReady;
	bool 		_requestParsed;
	bool 		_isCGI;
	LocationContext	_config_location;
	Client(int fd, Listen_Addr Client, Listen_Addr Server);
	in_addr_t 	getServerIp(void) const;
	in_addr_t 	getClientIp(void) const;
	int 		getServerPort() const ;
	int			getFd(void) const;
	int			getId(void) const;
	bool		is_Connected() const;
	void		set_Connect(bool);
	bool		Timeout(void) const;
	void		setReady(bool);
	bool		isReady(void) const;
	void 		getREQ(std::string);
	void 		reset(void);
	void 		getBody(std::string);
	void		matchLocation(std::vector<LocationContext>);
	void		createUploadFile(std::string filename, std::string content);
	void		generateResponse(Request& request, std::string path, int code);
	void		generateRedirectionResponse(Request& request, std::string path, int code);
	void 		handleRequestMethod(void);
	void 		getLocations(Server&);
	void 		handleGetRequest(void);
	void 		handlePostRequest(void);
	void 		handleDeleteRequest(void);
	stringMap 	fetchCGIEnv();
	std::string getPath();
	void 		handleCGI();


};
