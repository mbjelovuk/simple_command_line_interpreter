#ifndef READER_H
#define READER_H

#include <fstream>
#include <string>
using namespace std;

class Reader {
protected:
	istream* myinput;
public:
	Reader(istream* input);
	virtual ~Reader();

	Reader(const Reader&) = delete;
	Reader& operator=(const Reader&) = delete;
	Reader(Reader&&) = delete;
	Reader& operator=(Reader&&) = delete;

	virtual string getNextLine();
	virtual bool endOfRead();
};

class ConsoleReader : public Reader {
public:
	ConsoleReader();
	string getInput();
};

class FileReader : public Reader {
public:
	FileReader(string filepath);
	~FileReader();
	string getFileContent();
};

#endif
