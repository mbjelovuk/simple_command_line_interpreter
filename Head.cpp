#include "Head.h"
#include "SyntaxError.h"
#include <iostream>
#include <string>
using namespace std;

Head::Head() {
	name = "head";
}
void Head::execute() {
	inputstr();
	process();
	outputstr();
}
void Head::process() {
	if (opt.empty() || opt[0] != 'n' || opt.length() > 7) {
		throw SyntaxError("option must start with -n and be max 7 characters long (including -n)");
	} else if (opt.length() == 1) {
		throw SyntaxError("missing number after -n");
	} else {
		opt = opt.substr(1);
		int num = atoi(opt.c_str());
		this->setCnt(num);
	}
	if (count == 0) {
		result = "";
		return;
	}

	string ptr = strsequence;
	int lines = 0;
	size_t pos = 0;

	while (pos < ptr.length()) {
		if (ptr[pos] == '\n') {
			lines++;
			if (lines == count) {
				pos++;
				break;
			}
		}
		pos++;
	}
	if (lines < count) {
		pos = ptr.length();
	}

	result = ptr.substr(0, pos);
}
