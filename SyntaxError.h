#ifndef SYNTAXERROR_H
#define SYNTAXERROR_H

#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error {
public:
    SyntaxError(const std::string& msg);
};

#endif
