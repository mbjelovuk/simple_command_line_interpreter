#include "Command.h"
#include "Reader.h"
#include "OSError.h"
#include "SemanticError.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

Command::Command(string args, string opt) : arg(args), opt(opt) {}

CommandC::CommandC(string args, string opt, IOStream in) : Command(args, opt) {
    instream = in; 
}
void CommandC::inputstr(){
    //get the string sequence from input stream
    string instr;
    string filee;
    ConsoleReader* cr = nullptr;
    FileReader* fr = nullptr;
    switch (instream) {
    case PIPE:
        return;
    case CONSOLE:
        cr = new ConsoleReader();
        instr = cr->getInput();
        delete cr;
        cr = nullptr;
        break;
    case ARG:
        if (arg.empty()) throw SemanticError("argument empty");
        instr = arg;
        break;
    case FFILE: 
        if (!infile.empty() && infile[0] == '<' && infile.size() > 1) {
            filee = infile.substr(1);
        } else if (!infile.empty()) {
            filee = infile;
        }

        if (filee.empty()) {
            throw SemanticError("file naming mistake");
            return;
        }
        fr = new FileReader(string(filee));
        instr = fr->getFileContent();
        delete fr;
        fr = nullptr;

        break;
    }
    if (!strsequence.empty()) strsequence.clear();
	this->strsequence = instr;
}
void CommandC::outputstr(){
    //print result sequence to output stream
    if (result.empty()) {
        switch (outstream) {
        case CONSOLE:
            cout << "no output" << endl;
            break;
        case FFILE:
            //do nothing
            return;
        }
        return;
    }
    switch (outstream) {
    case CONSOLE:
        cout << result << endl;
        break;
    case PIPE:
        return;
    case FFILE:
        string filee;
        bool truncc = false;
        if (outfile.rfind(">>", 0) == 0 && outfile.length() > 2) {
            filee = outfile.substr(2);
        } else if (outfile[0] == '>' && outfile.length() > 1 && outfile[1] != '>') {
            truncc = true;
            filee = outfile.substr(1);
        } else if (!outfile.empty()) {
            filee = outfile;
        }

        std::ofstream ooutfile;
        if (!truncc) {
            ooutfile.open(filee, ios::app);
        } else {
            ooutfile.open(filee, ios::trunc);
        }
        if (!ooutfile) {
            throw OSError("cannot create file");
        }
        ooutfile << result;
        ooutfile.close();
        break;
    }
}

CommandP::CommandP(const vector<Command*>& commands) : pipeline(commands) {}
CommandP::~CommandP() {
    for (auto cmd : pipeline) delete cmd;
}
void CommandP::execute() {
    if (pipeline.empty()) return;
    for (size_t i = 0; i < pipeline.size(); i++) {
        CommandC* cmd = dynamic_cast<CommandC*>(pipeline[i]);
        if (i > 0) {
            CommandC* prev = dynamic_cast<CommandC*>(pipeline[i - 1]);
            if (prev) {
                cmd->setStrseq(prev->getResult());
            }
        }
        cmd->execute();
    }
}

/*
//maybe in the future - if we would need to copy/move pipeline (Command.clone() would need implementation - pure abstract method)
CommandP::CommandP(const CommandP& other) {
    for (auto cmd : other.pipeline) {
        pipeline.push_back(cmd->clone());
    }
}
CommandP& CommandP::operator=(const CommandP & other) {
    if (this != &other) {
        for (auto cmd : pipeline) delete cmd;
        pipeline.clear();
        for (auto cmd : other.pipeline) {
            pipeline.push_back(cmd->clone());
        }
    }
    return *this;
}
CommandP::CommandP(CommandP&& other) noexcept : pipeline(std::move(other.pipeline)) {
    other.pipeline.clear();
}
CommandP& CommandP::operator=(CommandP&& other) noexcept {
    if (this != &other) {
        for (auto cmd : pipeline) delete cmd;
        pipeline = std::move(other.pipeline);
        other.pipeline.clear();
    }
    return *this;
}
*/
