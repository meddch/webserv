#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <fstream>
#include <sstream>	
#include <string>
#define ISFILE 1
#define ISDIR 2
#define ISNOTEXIST 3
#define EMPTY ""


#include "Parse.hpp"

int    			toInt(std::string str);
std::string     toString(int value);
in_addr_t   	toIPv4(std::string str);
std::string 	toIPString(in_addr_t ip);
bool			isAllDigit(std::string str);

Listen_Addr		getAddressFromFd(int fd);
in_addr_t 		toIpNum(std::string str);
int 			getResourceType(std::string path);

