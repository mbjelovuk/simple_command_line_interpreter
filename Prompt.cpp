#include "Prompt.h"
#include <string>
using namespace std;

Prompt::Prompt(string args){
	name = "prompt";
    arg = args;
}
void Prompt::execute() {
	promptc = arg;
}
