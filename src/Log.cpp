#include "Log.hpp"

std::string getTimeStamp();

void Log::err(const std::string &msg, std::exception * e)
{
	std::cerr << getTimeStamp();
	std::cerr << "Error:"
				<< msg;
	if (e)
		std::cerr << " : " << e->what() << std::endl;
	else
		std::cerr << std::endl;
}

void Log::out(const std::string &msg)
{
	std::cout << getTimeStamp();
	std::cout << msg << std::endl;
}

std::string getTimeStamp()
{
	struct tm *time_ptr;
	time_t curr_time;
	std::stringstream retval;

	curr_time = time(NULL);
	time_ptr = localtime(&curr_time);
	retval << std::setfill('0');
	retval << "[" << std::to_string(time_ptr->tm_year + 1900);
	retval << "-";
	retval << std::setw(2) << std::to_string(time_ptr->tm_mon + 1);
	retval << "-";
	retval << std::setw(2) << std::to_string(time_ptr->tm_mday);
	retval << " ";
	retval << std::setw(2) << std::to_string(time_ptr->tm_hour);
	retval << ":";
	retval << std::setw(2) << std::to_string(time_ptr->tm_min);
	retval << ":";
	retval << std::setw(2) << std::to_string(time_ptr->tm_sec);
	retval << "] ";
	retval << std::setfill(' ');
	return retval.str();
}
