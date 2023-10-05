#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

int toInt(string str)
{
	std::istringstream iss(str);
	int result;
	char remainingChar;

	// Check if there are leftover characters
	if (!(iss >> result) || (iss.get(remainingChar)))
		throw runtime_error("convert " + str + " toInt fails");

	// Check fail and edge cases
	if (result == 0 && str != "0" && str != "+0" && str != "-0")
		throw runtime_error("convert " + str + " toInt fails");
	if (result == INT_MAX && str != "2147483647" && str != "+2147483647")
		throw runtime_error("convert " + str + " toInt fails");
	if (result == INT_MIN && str != "-2147483648")
		throw runtime_error("convert " + str + " toInt fails");

	return result;
}

bool isAllDigit(string str)
{
	for (size_t i = 0; i < str.length(); i++) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

in_addr_t toIPv4(string str)
{
	in_addr_t result = 0;

	for (int i = 0; i < 4; i++) {
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

int main(int ac, char **av)
{
    string a(av[1]);
   try
   {
    std::cout << toIPv4(a) << std::endl;
   }
   catch(const std::exception& e)
   {
    std::cerr << e.what() << '\n';
   }
   
}