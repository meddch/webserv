#include "Webserv.hpp"
#include <cstdlib>
#include <sys/signal.h>

bool _running ;

void sig_handler(int signum)
{
	(void)signum;

	std::cout << "\nClosing the WEBSERVER" << std::endl;
   	_running = false;
}



int main(int ac ,char **av)
{
	if (ac > 2)
		return 1;

	const std::string filename((ac == 2) ? av[1] : "./config/default.config");

	try
	{
		Parse conf(filename);
		_running = true;
		Core serv(conf.GetConfig());
		serv.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	std::exit(0);
}