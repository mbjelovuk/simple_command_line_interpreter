#ifndef UNKNOWNCOMMANDEXCEPTION_H
#define UNKNOWNCOMMANDEXCEPTION_H

#include <stdexcept>
#include <string>

class UnknownCommandException : public std::runtime_error{
public:
	UnknownCommandException(const std::string& cmd);
};

#endif
