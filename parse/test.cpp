#include <netinet/in.h>
#include <iostream>
#include <sstream>
#include <exception>
#include <deque>
#include <fstream>

using namespace std;


deque<string>	GetTokens(const string& content)
{
    const string whitespaces(" \t\r\n"), spc_caratcter("{};");
	size_t pos = 0;
    deque<string> _tokens;


	while (pos < content.size())
	{
		if (spc_caratcter.find(content[pos]) != spc_caratcter.npos)
		{
			_tokens.push_back(string(1, content[pos]));
			pos++;
		}
		else if (whitespaces.find(content[pos]) != whitespaces.npos)
			pos++;
		else
		{
			size_t tokEnd = content.find_first_of(whitespaces + spc_caratcter, pos);
			if (tokEnd == string::npos)
				tokEnd = content.size();
			_tokens.push_back(content.substr(pos, tokEnd - pos));
			pos += _tokens.back().size();
		}
	}
    return _tokens;
}

int main(int ac, char **av)
{


    const string filename(av[1]);
    std::ifstream file(filename.c_str());
	if (file.fail())
		throw runtime_error("Failed to open file: " + filename);
    
	string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	GetTokens(content);

    deque<string> _tokens = GetTokens(content);
    
    for (deque<string>::iterator it = _tokens.begin() ; it != _tokens.end();it++)
        cout << *it << endl;
   
}