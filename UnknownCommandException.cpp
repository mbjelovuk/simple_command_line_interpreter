#include "UnknownCommandException.h"

#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

UnknownCommandException::UnknownCommandException(const string& cmd)
    : runtime_error("Unknown command: " + cmd) {}
