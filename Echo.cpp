#include "Echo.h"
#include <iostream>
using namespace std;

Echo::Echo() {
	name = "echo";
}
void Echo::execute() {
	inputstr();
	process();
	outputstr();
}
void Echo::process() {
	result = strsequence;
}
