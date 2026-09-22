#ifndef TIME_H
#define TIME_H

#include "Command.h"

class Time : public CommandC {
public:
	Time();
	void execute() override;
	void process() override;
};

#endif
