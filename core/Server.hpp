#pragma once

#include "../parse/Utils.hpp"
#include <string>

class Server
{
private:
	int 			_id;
	stringMap		_sessions;
public:
	ServerContext _config;
	Server(const ServerContext config);
	Server();

	Listen_Addr						getAddress() const;
	std::string            			getName() const;
	std::string             		getErrorPage(int code) const;  
	ssize_t                			getMaxBodySize() const;
	bool               				bodySizeAllowed(int bytes) const;
	std::vector<std::string>		getMethods() const;
	std::string						getRoot() const;
	int								getId() const;
	LocationContext 				&getLocation(std::string);
	bool 							uploadEnabled() const;
	LocationContext					&findLocation(std::string uri);
	void							addSession(std::string session_id, std::string username);
	void							removeSession(std::string session_id);
	bool							isSessionValid(std::string session_id);
	std::string						getSessionUsername(std::string session_id);
	

};