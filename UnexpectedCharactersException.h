#ifndef UNEXPECTEDCHARACTERSEXCEPTION_H
#define UNEXPECTEDCHARACTERSEXCEPTION_H

#include <stdexcept>
#include <string>
#include <set>

class UnexpectedCharactersException : public std::runtime_error {
public:
	UnexpectedCharactersException(const std::string& line, const std::set<int>& indexes);
};

#endif
