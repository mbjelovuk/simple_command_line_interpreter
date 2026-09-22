#ifndef PROMPT_H
#define PROMPT_H

#include "Command.h"
#include <string>

extern std::string promptc;

class Prompt : public Command {
public:
	Prompt(std::string args);
	void execute() override;
};

#endif
