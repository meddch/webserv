#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
 
using std::vector; 
using std::map; 
using std::string; 
typedef vector<string> stringVec;


struct LocationConf
{
    bool         		auto_index;
    stringVec     		allow_methods;
    stringVec     		indxs;
	map<int ,string>	redirs;

};

struct  ServerContext
{
    int        				port;
    string    				host;
    string      			root;
    string             		server_name;
    ssize_t             	cl_max_size;
    map<int ,string>		error_pages;
    vector<LocationConf>	locations;

};

