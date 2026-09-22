#include "Exit.h"

Exit::Exit() {
	name = "exit";
}
void Exit::execute() {
	exitProgram = true;
}
