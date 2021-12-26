#pragma once
#include <exception>
#include <string>

class Exceptions : public std::exception
{

	std::string error_message;
public:

	Exceptions() = default;

	~Exceptions() = default;

	Exceptions(const std::string& message) : error_message(message) {}

	const char* what() const override
	{
		return error_message.c_str();
	}
};