#ifndef BATCH_H
#define BATCH_H

#include "Command.h"

class Parser;

class Batch : public CommandC {
private:
	Parser& parser;
public:
	Batch(Parser& p);

	Batch(const Batch&) = delete;
	Batch& operator=(const Batch&) = delete;
	Batch(Batch&&) = delete;
	Batch& operator=(Batch&&) = delete;

	Parser& getParser() const { return parser; }

	void execute() override;
	void process() override;
};

#endif
