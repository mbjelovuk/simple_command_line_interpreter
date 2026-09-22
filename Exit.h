#ifndef EXIT_H
#define EXIT_H

#include "Command.h"

extern bool exitProgram;

class Exit : public Command {
public:
	Exit();
	void execute() override;
};

#endif
