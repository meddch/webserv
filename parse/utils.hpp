#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <fstream>
#include <sstream>	

#include "Parse.hpp"

int    		toInt(string str);
string      toString(int value);
in_addr_t   toIPv4(string str);
string toIPString(in_addr_t ip);
bool		isAllDigit(string str);

string		fullPath(string root, string path);
string		getExtension(string path);
Listen_Addr	getAddressFromFd(int fd);

