#include "Webserv.hpp"


int main(int ac ,char **av)
{
	if (ac > 2)
		return 1;

	const std::string filename((ac == 2) ? av[1] : "conf.config");

	try
	{
		Parse conf(filename);
		Core serv(conf.GetConfig());
		serv.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}