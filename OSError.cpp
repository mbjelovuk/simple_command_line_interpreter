#include "OSError.h"

#include <iostream>
#include <string>
using namespace std;

OSError::OSError(const string& msg)
	: runtime_error("OS error: " + msg) {}
