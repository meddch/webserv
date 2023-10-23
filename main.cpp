#include "Webserv.hpp"


int main(int ac ,char **av)
{
	if (ac != 2)
		return 1;

	const std::string filename(av[1]);

	try
	{
		Parse conf(filename);
		Core serv(conf.GetConfig());
		serv.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what();
	}
}