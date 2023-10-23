#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <fstream>
#include <sstream>	
#include <string>


#include "Parse.hpp"

int    			toInt(std::string str);
std::string     toString(int value);
in_addr_t   	toIPv4(std::string str);
std::string 	toIPString(in_addr_t ip);
bool			isAllDigit(std::string str);

std::string		fullPath(std::string root, std::string path);
std::string		getExtension(std::string path);
Listen_Addr		getAddressFromFd(int fd);

