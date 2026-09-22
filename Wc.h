#ifndef WC_H
#define WC_H

#include "Command.h"

class Wc : public CommandC {
public:
	Wc();
	void execute() override;
	void process() override;
};

#endif
