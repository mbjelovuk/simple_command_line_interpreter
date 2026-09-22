#ifndef COMMANDBUILDER_H
#define COMMANDBUILDER_H

#include <string>
#include <vector>

class Command;
class Parser;

class CommandBuilder {
private:
	Parser* parser;
	bool pipe, first;
	void parametrize(Command* c, std::vector<std::string>& lineparts, bool tr);
public:
	CommandBuilder(Parser* p, bool pipe, bool first);

	//copy and moving makes no sense, shouldn't be a problem but it's safer to forbid it 
	// (same goes for CommandP, Parser, Reader, Batch)
	CommandBuilder(const CommandBuilder&) = delete;
	CommandBuilder& operator=(const CommandBuilder&) = delete;
	CommandBuilder(CommandBuilder&&) = delete;
	CommandBuilder& operator=(CommandBuilder&&) = delete;

	Command* buildCommand(std::vector<std::string>& lineparts);
};

#endif
