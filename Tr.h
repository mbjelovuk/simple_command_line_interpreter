#ifndef TR_H
#define TR_H

#include "Command.h"
#include <string>

class Tr : public CommandC {
private:
	std::string what;
	std::string with;
	bool promptwhat = false;
	bool promptwith = false;
public:
	Tr();

	//prompt user to set what and with - an idea for future expansion
	void setPromptWhat(bool w) { promptwhat = w; }
	void setPromptWith(bool w) { promptwith = w; }

	void setWhat(std::string w) { what = w; }
	void setWith(std::string w) { with = w; }
	std::string getWhat() const { return what; }
	std::string getWith() const { return with; }

	void execute() override;
	void process() override;
};

#endif
 