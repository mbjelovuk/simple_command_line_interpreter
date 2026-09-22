#include "Truncate.h"
#include "SyntaxError.h"
#include "SemanticError.h"
#include "OSError.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Truncate::Truncate(string args) {
	name = "truncate";
	arg = args;
}
void Truncate::execute() {
	if (arg.empty()) {
		throw SyntaxError("expected filename, none given");
	}
	string ffile(arg);

	ifstream infile(ffile);
	if (!infile.good()) {
		throw SemanticError("file '" + ffile + "' doesn't exist in the working directory");
	}
	infile.close();
	ofstream outfile (ffile, ofstream::trunc);
	if (!outfile) {
		throw OSError("file contents cannot be deleted");
	}
	outfile.close();
}
