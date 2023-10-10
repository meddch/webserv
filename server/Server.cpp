#include "Server.hpp"


Server::Server(const ServerContext config) : _config(config)
{}

Listen_Addr Server::getAddress() const { return _config.address; }

string Server::getName() const { return _config.serverName; }

string Server::getErrorPage(int code) const {
	map<int, string>::const_iterator it = _config.errorPages.find(code);
	return it == _config.errorPages.end() ? "" : fullPath(ROOT, it->second);
}

int Server::getMaxBodySize() const { return _config.clientMaxBodySize; }


bool Server::bodySizeAllowed(int bytes) const
{
	return getMaxBodySize() == -1 || getMaxBodySize() >= bytes;
}

set<Listen_Addr> Server::getUniqueAddresses(vector<Server> servers)
{

	set<Listen_Addr> uniques;
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i].getAddress().ip == 0) {
			uniques.insert(servers[i].getAddress());
		}
	}

	for (size_t i = 0; i < servers.size(); i++) {
		Listen_Addr addr = servers[i].getAddress();
		if (addr.ip != 0) {
			set<Listen_Addr>::iterator it;
			for (it = uniques.begin(); it != uniques.end(); it++) {
				if (it->ip == 0 && it->port == addr.port) break;
			}
			if (it == uniques.end()) uniques.insert(addr);
		}
	}
	return uniques;
}