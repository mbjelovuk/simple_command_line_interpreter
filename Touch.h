#ifndef TOUCH_H
#define TOUCH_H

#include "Command.h"
#include <string>

class Touch : public Command {
public:
	Touch(std::string args);
	void execute() override;
};

#endif
