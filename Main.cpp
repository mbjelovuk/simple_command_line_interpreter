#include "Parser.h"
#include "Command.h"
#include "UnexpectedCharactersException.h"
#include "UnknownCommandException.h"
#include "SyntaxError.h"
#include "SemanticError.h"
#include "OSError.h"
#include <iostream>
#include <string>
using namespace std;

string promptc = "$";
bool exitProgram = false;

int main(){
	Command* com = nullptr;
	ParserA* p = new ParserA(new ConsoleReader());
	char line[513];
	while (!exitProgram) {
		cout << promptc + ' ';
		cin.getline(line, 513);
		if (cin.eof()) {
			delete p;
			return 0;
		}
		try {
			com = p->parseLine(string(line));
			if (!com) {
				continue;
			} else {
				com->execute();
			}
		} catch (const UnexpectedCharactersException& e) {
			cerr << e.what() << endl;
		} catch (const UnknownCommandException& e) {
			cerr << e.what() << endl;
		} catch (const SyntaxError& e) {
			cerr << e.what() << endl;
		} catch (const SemanticError& e) {
			cerr << e.what() << endl;
		} catch (const OSError& e) {
			cerr << e.what() << endl;
		} catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
		}
		delete com;
		com = nullptr;
		cin.clear();
	}
	delete p;

	return 0;
}
