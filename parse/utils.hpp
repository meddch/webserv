#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <fstream>
#include <sstream>	

#include "Config.hpp"

string getFileContent(string path);
string toString(int value);
string toIPString(in_addr_t ip);
in_addr_t toIPv4(string str);
int toInt(string str);
bool isAllDigit(string str);

string fullPath(string root, string path);
string getExtension(string path);

Address getAddressFromFd(int fd);
