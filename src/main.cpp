#include "Server.hpp"

void	app(std::string port, std::string pass);

int main(int argc, char **argv)
{
	if (argc != 3)
		std::cerr << "USAGE: ircserver PORT PASS" << std::endl;
	else
		app(argv[1], argv[2]);
}

void	app(std::string port, std::string pass)
{
	Server server(port, pass);

	server.run();
}

