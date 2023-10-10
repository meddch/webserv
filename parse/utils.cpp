#include "ParseConfig.hpp"

string toString(int value)
{
	std::stringstream ss;
	ss << value;
    
	if (ss.fail())
        return "";
	return ss.str();
}

int toInt(string str)
{
	std::istringstream iss(str);
	int result;
	char remainingChar;

	if (!(iss >> result) || (iss.get(remainingChar))) 
		throw runtime_error("convert " + str + " toInt fails");

	if (result == 0 && str != "0" && str != "+0" && str != "-0")
		throw runtime_error("convert " + str + " toInt fails");

	if (result == INT_MAX && str != "2147483647" && str != "+2147483647")
		throw runtime_error("convert " + str + " toInt fails");

	if (result == INT_MIN && str != "-2147483648")
		throw runtime_error("convert " + str + " toInt fails");

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


bool isAllDigit(string str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (!std::isdigit(str[i]))
			return false;
	return true;
}


in_addr_t toIPv4(string str)
{
	in_addr_t result = 0;

	for (int i = 0; i < 4; i++)
    {
		if (i < 3 && str.find('.') == string::npos)
			throw runtime_error("failed to convert " + str);

		string token = i < 3 ? str.substr(0, str.find('.')) : str;
		int value = toInt(token);
		if (!isAllDigit(token) || value < 0 || value > 255)
			throw runtime_error("failed to convert " + str);

		result = (result << 8) | value;

		if (i < 3)
			str.erase(str.begin(), str.begin() + str.find('.') + 1);
	}

	return htonl(result);
}


string fullPath(string root, string path)
{

	try
    {
		// Remove '/' from root and path
		root = root.at(root.size() - 1) == '/' ? root.substr(0, root.size() - 1) : root;
		path = path.at(0) == '/' ? path.substr(1) : path;
	}
	catch (...)
    {
		// Protect against if strings are empty or index out of bound
		return "";
	}
	return root + "/" + path;
}

string getExtension(string path)
{
	size_t pos = path.find_last_of('.');

	if (pos != string::npos && pos != 0)
		return path.substr(pos);

	return "";
}

Listen_Addr getAddressFromFd(int fd)
{
	struct sockaddr_in serverAddress;
	socklen_t addrLen = sizeof(serverAddress);
	if (getsockname(fd, (struct sockaddr*)&serverAddress, &addrLen) == -1)
		throw runtime_error("getsockname() failed");

	Listen_Addr addr;
	addr.ip = ntohl(serverAddress.sin_addr.s_addr);
	addr.port = ntohs(serverAddress.sin_port);
	return addr;
}

