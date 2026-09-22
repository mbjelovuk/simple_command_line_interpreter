#include "SemanticError.h"

#include <iostream>
#include <string>
using namespace std;

SemanticError::SemanticError(const string& msg)
	: runtime_error("Semantic error: " + msg) {}
