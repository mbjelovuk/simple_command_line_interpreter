#ifndef SEMANTICERROR_H
#define SEMANTICERROR_H

#include <stdexcept>
#include <string>

class SemanticError : public std::runtime_error {
public:
    SemanticError(const std::string& msg);
};

#endif
