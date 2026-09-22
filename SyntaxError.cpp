#include "SyntaxError.h"

#include <iostream>
#include <string>
using namespace std;

SyntaxError::SyntaxError(const string& msg) 
	: runtime_error("Syntax error: " + msg) {}
