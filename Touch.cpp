#include "Touch.h"
#include "SyntaxError.h"
#include "SemanticError.h"
#include "OSError.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Touch::Touch(string args) {
	name = "touch";
	arg = args;
}
void Touch::execute() {
	if (arg.empty()) {
		throw SyntaxError("expected filename, none given");
	}
	string ffile(arg);

	ifstream infile(ffile);
	if (infile.good()) {
		throw SemanticError("file '" + ffile + "' already exists");
	}
	infile.close();
	ofstream outfile(ffile);
	if (!outfile) {
		throw OSError("cannot create file: '" + ffile + "'");
	}
	outfile.close();
}
