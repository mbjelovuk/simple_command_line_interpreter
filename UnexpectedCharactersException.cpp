#include "UnexpectedCharactersException.h"

#include <sstream>
#include <stdexcept>
#include <set>
#include <string>
using namespace std;

static string message(const string& line, const set<int>& indexes) {
    ostringstream oss;
    oss << "Error - Unexpected characters:\n";
    oss << line << endl;
    for (int i = 0; i < line.length(); i++) {
        oss << (indexes.count(i) ? "^" : " ");
    }
    return oss.str();
}

UnexpectedCharactersException::UnexpectedCharactersException(const string& line, const set<int>& indexes) 
	: runtime_error(message(line, indexes)) {}
