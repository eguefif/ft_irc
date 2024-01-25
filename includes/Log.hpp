#pragma once
#include <iostream>
#include <exception>
#include <ctime>


class Log
{
	public:
		static void err(const std::string &msg, std::exception * e);
		static void out(const std::string &msg);
	
	private:
		Log() {}
		~Log() {}
};
