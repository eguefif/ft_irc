#include "../includes/Log.hpp"

void getTimeStamp();

void Log::err(const std::string &msg, std::exception * e)
{
	//std::cerr << getTimeStamp()
	std::cerr << " | Error:"
	<< msg;
	if (e)
		std::cerr << " : " << e->what() << std::endl;
	else
		std::cerr << std::endl;
}

void Log::out(const std::string &msg)
{
	std::cout << msg << std::endl;
}

void getTimeStamp()
{
}
