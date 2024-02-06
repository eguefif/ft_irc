#include "Server.hpp"

bool isRunning = true;

Server::Server(const std::string &pPort, const std::string &pPass)
{
	this->pass = pPass;
	this->setPort(pPort);
	this->numSockets = 0;
	this->initServerSocket();
	this->initPoll();
	this->initSignal();
}

void Server::setPort(const std::string &pPort)
{
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
}

void Server::initServerSocket()
{
	this->createServerSocket();
	this->setAddressServerStruct();
	this->runBind();
	this->runListen();
}

void Server::createServerSocket()
{
	this->createSocket();
	this->setupSocket();
	this->setNonBlockingSocket(this->serverSocket);
	this->numSockets++;
}

void Server::createSocket()
{
	if ((this->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		Log::err("socket failed", 0);
		exit(1);
	}
}

void Server::setupSocket()
{
	int opt = 1;

	if (setsockopt(this->serverSocket, SOL_SOCKET,
				SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		Log::err("setsockopt failed", 0);
		exit(1);
	}
}

void Server::setNonBlockingSocket(const int &fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		Log::err("fcntl error: " + std::to_string(errno), 0);
}

void Server::setAddressServerStruct()
{
	this->addressServer.sin_family = AF_INET;
	if (!inet_aton(HOST, &(this->addressServer.sin_addr)))
	{
		Log::err("invalid IP this->addressServer", 0);
		exit(1);
	}
	this->addressServer.sin_port = htons(this->port);
}


void Server::runBind()
{
	if (bind(this->serverSocket, (struct sockaddr *) &this->addressServer, sizeof(this->addressServer)) < 0)
	{
		Log::err("bind failed", 0);
		exit(1);
	}
}

void Server::runListen()
{
	if (listen(serverSocket, 10) < 0)
	{
		Log::err("listen failed", 0);
		exit(1);
	}
}

void Server::initPoll()
{
	struct pollfd p;

	p.fd = this->serverSocket;
	p.events = POLLIN;
	p.revents = 0;
	this->pfds.push_back(p);
}

void Server::initSignal()
{
	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);
	signal(SIGTERM, handleSignal);

}

Server::~Server()
{
	for (nfds_t i = 0; i < this->numSockets; ++i)
		close(this->pfds[i].fd);
}
void Server::exitGracefully()
{
	for (std::map<int, Client *>::iterator it = this->clientList.begin();
			it != this->clientList.end();
			++it)
		delete it->second;
	for (std::map<std::string, Channel *>::iterator it = this->channelList.begin();
			it != this->channelList.end();
			++it)
		delete it->second;
	for (std::vector<pollfd>::iterator it = this->pfds.begin();
			it != this->pfds.end();
			++it)
	{
		std::string msg = "ERROR: Quit: server is shutting down";
		int fd = it->fd;
		write(fd, msg.c_str(), msg.length());
		write(fd, "\015\n", 2);
		close(fd);
	}
}

void handleSignal(int sig)
{
	(void) sig;
	isRunning = false;
}
