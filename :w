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
