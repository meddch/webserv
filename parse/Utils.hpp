#pragma once

#include <sys/_types/_size_t.h>
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
#define REDIRECT "301"
#define HTTP "http://localhost:"


#include "Parse.hpp"

int    			toNum(std::string str);
std::string     toString(int value);
in_addr_t   	toIPv4(std::string str);
std::string 	toIPString(in_addr_t ip);
bool			isAllDigit(std::string str);

Listen_Addr		getAddressFromFd(int fd);
in_addr_t 		toIpNum(std::string str);
int 			getResourceType(std::string path);
std::string 	getRootPath();

