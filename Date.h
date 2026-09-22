#ifndef DATE_H
#define DATE_H

#include "Command.h"

class Date : public CommandC {
public:
	Date();
	void execute() override;
	void process() override;
};

#endif
