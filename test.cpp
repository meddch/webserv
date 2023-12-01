#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <deque>
#include <fstream>
#include <stdexcept>

using namespace std;



int main(int ac, char **av)
{


	std::cout << realpath("../../../../../h", NULL) << std::endl;
   
}


			struct stat filestat;
			char buffer[BYTES];
			stat(client.response.filePath.c_str(), &filestat);
			client.response._fileSize = filestat.st_size;
			std::fstream filedata;
			filedata.open(client.response.filePath.c_str(), std::ios::in | std::ios::binary);
			filedata.seekg(client.response._offset, std::ios::beg);
			std::stringstream ss;
			ss << client.request._headers["range"].substr(client.request._headers["range"].find("=") + 1);
			ss >> client.response._offset;;
			filedata.read(buffer, BYTES);
			std::string Str = gener




			else 
		{
			FILE *file = fopen(client.response.filePath.c_str(), "r");
			std::fseek(file, 0, SEEK_END);
			client.response._fileSize = std::ftell(file);
			std::cout << client.request._headers["range"] << std::endl;
			std::stringstream ss;
			ss << client.request._headers["range"].substr(client.request._headers["range"].find("=") + 1);
			ss >> client.response._offset;
			std::fseek(file, client.response._offset, SEEK_SET);
			char buffer[BYTES];
			ssize_t bytesRead = std::fread(buffer, 1, BYTES, file);
			std::fclose(file);
			client.response.generateChunkedResponse();
			Str = client.response.response;
			Str += std::string(buffer, bytesRead);
			std::cout << Str << std::endl;
			bytesSent = send(client.getFd(), Str.c_str(), Str.length(), 0);
			if (bytesRead < BYTES)
				client.set_Connect(false);
			if (bytesRead == -1 || bytesRead == 0)
			{
				client.set_Connect(false);
				std::cout << "Client " << client.getId() << " disconnected" << std::endl;
				client.setReady(false);
				close(client.getFd());
				return;
			}
	
		}