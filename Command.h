#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
using namespace std;

enum IOStream {
	CONSOLE,
	ARG,
	FFILE,
	PIPE
};
 
class Command {
protected:
	string name;
	string opt;
	string arg;
public:
	Command() {};
	Command(string args, string opt);
	virtual ~Command() {};

	string getName() const { return name; }
	string getArg() const { return arg; }
	string getOpt() const { return opt; }
	void setName(string n) { name = n; }
	void setArg(string a) { arg = a; }
	void setOpt(string o) { opt = o; }

	virtual void execute() = 0; 
};

//Command that works with character sequence
class CommandC : public Command {
protected:
	IOStream instream = CONSOLE;
	IOStream outstream = CONSOLE;
	string strsequence;
	string result;
	string infile;
	string outfile;
public:
	CommandC() {};
	CommandC(string args, string opt, IOStream in);

	string getStrseq() const { return strsequence; }
	string getResult() const { return result; }
	void setStrseq(string s) { strsequence = s; }
	void setResult(string s) { result = s; }
	IOStream getInstream() const { return instream; }
	IOStream getOutstream() const { return outstream; }
	void setInstream(IOStream i, string filename) { instream = i; infile = filename; }
	void setOutstream(IOStream i, string filename) { outstream = i; outfile = filename; }

	void inputstr();
	void outputstr();
	virtual void process() = 0;   //result = ...
};

//Pipeline of commands that work with character sequences
class CommandP : public Command {
private:
	vector<Command*> pipeline;
public:
	CommandP(const vector<Command*>& commands);
	~CommandP();

	CommandP(const CommandP&) = delete;
	CommandP& operator=(const CommandP&) = delete;
	CommandP(CommandP&&) = delete;
	CommandP& operator=(CommandP&&) = delete;

	void execute() override;
};
#endif
