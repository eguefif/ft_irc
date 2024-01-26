#include "ACmd.hpp"

ACmd::ACmd(const int &pFd, const std::string pMessage) :
	fd(pFd), message(pMessage)
{}


ACmd::ACmd(const ACmd &other) : fd(other.fd), message(other.message)
{
}

ACmd &ACmd::operator=(const ACmd &other)
{
	(void) other;
	return *this;
}
