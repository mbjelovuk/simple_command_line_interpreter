#ifndef TRUNCATE_H
#define TRUNCATE_H

#include "Command.h"
#include <string>

class Truncate : public Command {
public:
	Truncate(std::string args);
	void execute() override;
};

#endif

