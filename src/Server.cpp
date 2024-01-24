#include "../includes/Server.hpp"

Server::Server(const std::string &pPort, const std::string &pPass)
{
	this->pass = pPass;
	try
	{
		this->port = std::stoi(pPort);
	}
	catch(std::exception & e)
	{
		Log::err("port param error", &e);
	}
	if (port < 0 || port > 49151)
		Log::err("port is out of range [0;49151]", 0);
	serverSocket = Server::initConnection(this->addressServer);
}

int Server::initConnection(struct sockaddr_in &address)
{
	int newSocket;
	int opt = 1;

	if ((newSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Log::err("socket failed", 0);
	}
	if (setsockopt(newSocket, SOL_SOCKET,
				SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		Log::err("setsockopt failed", 0);
	}
	address.sin_family = AF_INET;
	if (!inet_aton("127.0.0.1", &(address.sin_addr)))
	{
		Log::err("invalid IP address", 0);
	}
	address.sin_port = htons(this->port);
	if (bind(newSocket, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		std::cout << errno << std::endl;
		Log::err("bind failed", 0);
	}
	if (listen(newSocket, 10) < 0)
	{
		Log::err("listen failed", 0);
	}
	return newSocket;
}

void Server::run()
{
	struct sockaddr address;
	socklen_t addrlen = sizeof(sockaddr);
	while (true)
	{
		accept(this->serverSocket, (struct sockaddr *)&address, &addrlen);
		std::string str(inet_ntoa(this->addressServer.sin_addr));
		Log::out("new connection with " + str + ":" + std::to_string(ntohs(this->addressServer.sin_port)));
	}
}
