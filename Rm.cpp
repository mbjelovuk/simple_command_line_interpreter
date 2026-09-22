#include "Rm.h"
#include "SemanticError.h"
#include "SyntaxError.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Rm::Rm(string args) {
	name = "rm";
	arg = args;
}
void Rm::execute() {
	if (arg.empty()) {
		throw SyntaxError("expected filename, none given");
	}
	string ffile(arg);

	if (remove(ffile.c_str()) != 0) {
		throw SemanticError("file '" + ffile + "' doesn't exist or cannot be deleted");
	}
}
