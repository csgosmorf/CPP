#include<string>
#include<iostream>
#include "my_std_lib_facilities.h"

void error(const std::string& s)
{
	throw std::runtime_error(s);
}

void error(const std::string& s, const std::string& s2)
{
	error(s+s2);
}

void keep_window_open()
{
	std::cin.clear();
	std::cout << "Please enter a character to exit\n";
	char ch;
	std::cin >> ch;
	return;
}

void keep_window_open(std::string s)
{
	if (s=="") return;
	std::cin.clear();
	std::cin.ignore(120,'\n');
	for (;;) {
		std::cout << "Please enter " << s << " to exit\n";
		std::string ss;
		while (std::cin >> ss && ss!=s)
			std::cout << "Please enter " << s << " to exit\n";
		return;
	}
}