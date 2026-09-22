#ifndef ECHO_H
#define ECHO_H

#include "Command.h"

class Echo : public CommandC {
public:
	Echo();
	void execute() override;
	void process() override;
}; 

#endif
