#include "Wc.h"
#include "SemanticError.h"
#include <iostream>
#include <cctype>
#include <string>
using namespace std;

static int countWords(string input) {
    if (input.empty()) return 0;
    bool word = false;
    int cnt = 0;
    for (char ch : input) {
        if (isspace(static_cast<unsigned char>(ch))) {
            word = false;
        } else if (!word) {
            cnt++;
            word = true;
        }
    }
    return cnt;
}
static size_t countChars(string input) {
    return input.length();
}

Wc::Wc() {
    name = "wc";
}
void Wc::execute() {
	inputstr();
	process();
	outputstr();
}
void Wc::process() {
    int res = 0;
    if (opt == "w") {
        res = countWords(strsequence);
    } else if (opt == "c") {
        res = static_cast<int>(countChars(strsequence));
    } else throw SemanticError("unknown option: -" + opt);
    result = to_string(res);
}
