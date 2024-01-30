#include "stringUtils.hpp"

std::string trimString(std::string str)
{
	std::string::iterator it = str.begin();

	while (it != str.end() && (*it == ' ' || *it == '\n'))
	{
		str.erase(it);
		++it;
	}
	it = str.end();
	while (it != str.begin() && (*it == ' ' || *it == '\n'))
	{
		str.erase(it);
		--it;
	}
	return str;
}

bool isPrint(std::string &str)
{
	for(std::string::iterator it = str.begin();
			it != str.end();
			++it)
	{
		 if (!std::isprint(*it) || *it == ',')
			 return false;
	}
	return true;
}

bool isWord(std::string str)
{
	for(std::string::iterator it = str.begin();
			it != str.end();
			++it)
	{
		 if (!std::isprint(*it) || *it == ' ')
			 return false;
	}
	return true;
}
