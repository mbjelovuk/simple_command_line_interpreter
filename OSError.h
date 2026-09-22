#ifndef OSERROR_H
#define OSERROR_H

#include <stdexcept>
#include <string>

class OSError : public std::runtime_error {
public:
    OSError(const std::string& msg);
};

#endif
