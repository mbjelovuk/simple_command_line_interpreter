#include "Tr.h"
#include "SemanticError.h"
#include <iostream>
#include <string>
using namespace std;

//prompt user and get input
static string readlinetochar(const char* prompt) {
    cout << prompt << endl;
    string buffer;
    getline(cin, buffer);
    return buffer;
}

Tr::Tr() {
    name = "tr";
}
void Tr::execute() {
	inputstr();

    //user manually sets what and with
    if (promptwhat) {
        setWhat(readlinetochar("Input characters you want to replace:"));
    }
    if (promptwith) {
        string p = string("Input characters you want to replace '") + getWhat() + "' with:";
        setWith(readlinetochar(p.c_str()));
    }

	process();
	outputstr();
}
void Tr::process() {
    if (what.empty()) throw SemanticError("'what' not stated");;

    string input(strsequence);
    string search(what);
    string replace = !with.empty() ? string(with) : "";

    size_t pos = 0;
    while ((pos = input.find(search, pos)) != string::npos) {
        input.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    result = input;
}
