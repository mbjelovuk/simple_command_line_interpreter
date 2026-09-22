#include "Parser.h"
#include "Batch.h"
#include "Time.h"
#include "Date.h"
#include "UnexpectedCharactersException.h"
#include "SyntaxError.h"
#include "SemanticError.h"
#include <string>
#include <vector>
#include <regex>
#include <set>
using namespace std;

static void validChars(string line) {
	const string allowed = "abcdefghijklmnopqrstuvwxyz0123456789/.-<>\"| \t";
	set<int> indexes;
	bool inquotes = false;
	int openquotesindex = -1;
	for (int i = 0; i < line.length(); i++) {
		char ch = line[i];
		if (ch == '"') {
			inquotes = !inquotes;
			if (inquotes) openquotesindex = i;
			continue;
		}
		if (!inquotes && allowed.find(ch) == string::npos) {
			indexes.insert(i);
		}
	}
	if (inquotes && openquotesindex != -1) {
		indexes.insert(openquotesindex);
		//cout << "Error - unclosed quotes" << endl;  //maybe clearer error description
	}
	if (!indexes.empty()) throw UnexpectedCharactersException(line, indexes);
}

Parser::Parser(Reader* reader) : myreader(reader) {}
Parser::~Parser() {
	delete myreader;
}
Command* Parser::getNextCom() {
	string line = myreader->getNextLine();
	if (line.empty() || myreader->endOfRead()) return nullptr;
	return parseLine(line);
}

ParserA::ParserA(Reader* reader) : Parser(reader) {}
Command* ParserA::parseSingleCom(string line, bool pipe, bool first) {
	if (line.empty()) return nullptr;
	Command* c = nullptr;
	vector<string> lineparts;
	bool inquotes = false;
	string current;
	for (size_t i = 0; i < line.size(); i++) {
		char ch = line[i];
		if (ch == '"') {
			inquotes = !inquotes;
			current += ch;
		} else if (isspace(static_cast<unsigned char>(ch)) && !inquotes) {
			if (!current.empty()) {
				lineparts.push_back(current);
				current.clear();
			}
		} else {
			current += ch;
		}
	}
	if (!current.empty()) {
		lineparts.push_back(current);
	}
	if (lineparts.empty()) {
		return nullptr;
	}
	size_t n = lineparts.size();

	CommandBuilder cb(this, pipe, first);   //without copy/move constructors
	//CommandBuilder cb = CommandBuilder(this, pipe, first);  //copy or move constructor must be implemented
	
	return cb.buildCommand(lineparts);
}
Command* ParserA::parseLine(string line) {
	if (line.empty() || line.find_first_not_of(" \t\n\r\v\f") == string::npos) return nullptr;
	validChars(line);

	vector<string> pipeparts;
	regex rex(R"(\s*\|\s*)");
	sregex_token_iterator it(line.begin(), line.end(), rex, -1);
	sregex_token_iterator end;

	for (; it != end; ++it) {
		if (it->str().find_first_not_of(" \t\r\n") == string::npos) {
			throw SyntaxError("empty command in pipeline");
		}
		pipeparts.push_back(*it);
	}

	//when '|' is the last non-white character in line
	size_t lastpipe = line.rfind('|');
	if (lastpipe != string::npos) {
		size_t firstNonWAfter = line.find_first_not_of(" \t\r\n", lastpipe + 1);
		if (firstNonWAfter == string::npos) {
			throw SyntaxError("empty command in pipeline");
		}
	}
	
	if (pipeparts.size() == 1) {
		return parseSingleCom(pipeparts[0], false, false);
	}

	vector<Command*> pipeline;
	try {
		for (size_t i = 0; i < pipeparts.size(); i++) {
			Command* cmd = parseSingleCom(pipeparts[i], true, (i==0));
			CommandC* c = dynamic_cast<CommandC*>(cmd);
			if (!c) throw SemanticError("commands that don't have input/output stream cannot be parts of a pipeline");
			if (dynamic_cast<Batch*>(c) && i != pipeparts.size() - 1) throw SemanticError("command 'batch' must be at the end of a pipeline (it has no output stream)");
			IOStream in = c->getInstream();
			IOStream out = c->getOutstream();
			if (i > 0) {
				if (dynamic_cast<Time*>(c) || dynamic_cast<Date*>(c)) throw SemanticError("commands that don't have input stream must be at the beginning of a pipeline");
				if (in == ARG || in == FFILE) throw SemanticError("only the first command in a pipeline may have input specified");
				c->setInstream(PIPE, "");
			}
			if (i < pipeparts.size() - 1) {
				if (out == FFILE) throw SemanticError("only the last command in a pipeline may have output specified");
				c->setOutstream(PIPE, "");
			}
			pipeline.push_back(cmd);
		}
	} catch (...) {
		for (Command* cmd : pipeline) delete cmd;
		throw; 
	}

	return new CommandP(pipeline);
}
