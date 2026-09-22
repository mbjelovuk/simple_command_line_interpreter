#ifndef RM_H
#define RM_H

#include "Command.h"
#include <string>

class Rm : public Command {
public:
	Rm(std::string args);
	void execute() override;
};

#endif
