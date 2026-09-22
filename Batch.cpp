#include "Batch.h"
#include "Parser.h"
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

Batch::Batch(Parser& p) : parser(p) {
    name = "batch";
}
void Batch::execute() {
    inputstr();
    process();
    //outputstr();   //batch has no result string, it's output redirection is actually for inner commands
}
void Batch::process() {
    if (strsequence.empty()) return;

    istringstream iss(strsequence);
    string lline;

    while (getline(iss, lline)) {
        if (lline.empty()) continue;

        Command* cmd = nullptr;
        try {
            cmd = parser.parseLine(lline);
            CommandC* c = dynamic_cast<CommandC*>(cmd);
            if (cmd) {
                if (c && !this->outfile.empty() && c->getOutstream()!=FFILE) {
                    c->setOutstream(FFILE, outfile);
                }
                cmd->execute();
                delete cmd;
            } else {
                cout << "error: failed to parse command: " << lline << endl;
            }
        } catch (const exception& e) {
            cout << "exception while processing line: " << lline << "\n"
                << "reason: " << e.what() << endl;
            delete cmd;
        }
    }
}
