#include "Reader.h"
#include "OSError.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;


Reader::Reader(istream* input) : myinput(input) {}

Reader::~Reader() {}

string Reader::getNextLine() {
	string line;
	getline(*myinput, line);
	return line;
}
bool Reader::endOfRead() {
	return myinput->eof();
}


FileReader::FileReader(string filepath) : Reader(new ifstream(filepath)) {}

FileReader::~FileReader() {
	((ifstream*)myinput)->close();
	delete myinput;
}

string FileReader::getFileContent() {
	if (!myinput || !((ifstream*)myinput)->is_open()) {
		throw OSError("cannot open file");
	}
	myinput->clear();
	myinput->seekg(0, ios::beg);

	ostringstream buffer;
	buffer << myinput->rdbuf();

	return buffer.str();
}


ConsoleReader::ConsoleReader() : Reader(&cin) {}

string ConsoleReader::getInput() {
	string input;
	while (!endOfRead()) {
		string line = getNextLine();
		input += line + '\n';
	}
	return input;
}
