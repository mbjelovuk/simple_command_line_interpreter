#ifndef PARSER_H
#define PARSER_H

#include "Reader.h"
#include "Command.h"
#include "CommandBuilder.h"
#include <string>
using namespace std;

class Parser {
protected:
	Reader* myreader = nullptr;
public:
	Parser(Reader* reader);
	virtual ~Parser();

	Parser(const Parser&) = delete;
	Parser& operator=(const Parser&) = delete;
	Parser(Parser&&) = delete;
	Parser& operator=(Parser&&) = delete;

	virtual Command* parseLine(string line) = 0;
	Command* getNextCom();

	friend bool quotesCheck(string str);
	friend string removeQuotes(const string& str);
};

class ParserA : public Parser {
public:
	ParserA(Reader* reader);

	Command* parseLine(string line) override;
	virtual Command* parseSingleCom(string line, bool pipe, bool first);
};

#endif
