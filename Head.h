#ifndef HEAD_H
#define HEAD_H

#include "Command.h"

class Head : public CommandC {
private:
	int count = 0;
public:
	Head();

	int getCnt() const { return count; }
	void setCnt(int c) { count = c; }

	void execute() override;
	void process() override;
};

#endif
