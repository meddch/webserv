#include "Utils.hpp"
#include <sys/_types/_ssize_t.h>


std::string toString(int value)
{
	std::stringstream ss;
	ss << value;
    
	if (ss.fail())
        return "";
	return ss.str();
}

int toNum(std::string str)
{
	std::istringstream iss(str);
	int result;
	char remainingChar;

	if (!(iss >> result) || (iss.get(remainingChar))) 
		throw std::runtime_error("convert " + str + " toInt fails");

	if (result == 0 && str != "0" && str != "+0" && str != "-0")
		throw std::runtime_error("convert " + str + " toInt fails");

	if (result == INT_MAX && str != "2147483647" && str != "+2147483647")
		throw std::runtime_error("convert " + str + " toInt fails");

	if (result == INT_MIN && str != "-2147483648")
		throw std::runtime_error("convert " + str + " toInt fails");

	return result;
}

std::string toIPString(in_addr_t ip)
{
	std::ostringstream oss;
	unsigned char* bytes = reinterpret_cast<unsigned char*>(&ip);

	oss << static_cast<int>(bytes[0]) << '.'
		<< static_cast<int>(bytes[1]) << '.'
		<< static_cast<int>(bytes[2]) << '.'
		<< static_cast<int>(bytes[3]);

	return oss.str();
}


bool isAllDigit(std::string str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isdigit(str[i]))
			return false;
	return true;
}


in_addr_t toIPv4(std::string str)
{
	in_addr_t result = 0;

	for (int i = 0; i < 4; i++)
    {
		if (i < 3 && str.find('.') == std::string::npos)
			throw std::runtime_error("failed to convert " + str);

		std::string token = i < 3 ? str.substr(0, str.find('.')) : str;
		int value = toNum(token);
		if (!isAllDigit(token) || value < 0 || value > 255)
			throw std::runtime_error("failed to convert " + str);

		result = (result << 8) | value;

		if (i < 3)
			str.erase(str.begin(), str.begin() + str.find('.') + 1);
	}

	return htonl(result);
}

std::string getRootPath()
{
	char *realPat = realpath(".", NULL);
	if (realPat == NULL)
		throw std::runtime_error("Parser :: failed to get root path");
	std::string realPath = realPat + std::string(ROOT);
	free(realPat);
	return realPath;
}


Listen_Addr getAddressFromFd(int fd)
{
	struct sockaddr_in serverAddress;
	socklen_t addrLen = sizeof(serverAddress);
	if (getsockname(fd, (struct sockaddr*)&serverAddress, &addrLen) == -1)
		throw std::runtime_error("getsockname() failed");

	Listen_Addr addr;
	addr.ip = ntohl(serverAddress.sin_addr.s_addr);
	addr.port = ntohs(serverAddress.sin_port);
	return addr;
}



in_addr_t toIpNum(std::string str)
{
	in_addr_t result = 0;

	for (int i = 0; i < 4; i++) {
		if (i < 3 && str.find('.') == std::string::npos)
			throw std::runtime_error("failed to convert " + str);

		std::string token = i < 3 ? str.substr(0, str.find('.')) : str;
		int value = toNum(token);
		if (!isAllDigit(token) || value < 0 || value > 255)
			throw std::runtime_error("failed to convert " + str);

		result = (result << 8) | value;
		if (i < 3)
			str.erase(str.begin(), str.begin() + str.find('.') + 1);
	}

	return htonl(result);
}

int getResourceType(std::string path)
{
	struct stat path_stat;
    stat(path.c_str(), &path_stat);
    bool isFile = S_ISREG(path_stat.st_mode);
    bool isDir = S_ISDIR(path_stat.st_mode);

	if (isFile)
		return ISFILE;
	if (isDir)
		return ISDIR;
	return ISNOTEXIST;
}