#include "CommandBuilder.h"
#include "Parser.h"
#include "Echo.h"
#include "Exit.h"
#include "Prompt.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Truncate.h"
#include "Rm.h"
#include "Wc.h"
#include "Tr.h"
#include "Head.h"
#include "Batch.h"
#include "UnexpectedCharactersException.h"
#include "UnknownCommandException.h"
#include "SyntaxError.h"
#include "SemanticError.h"
#include <vector>
#include <string>
using namespace std;

static bool quotesCheck(string str) {
	if (str.empty()) return false;
	size_t len = str.length();
	if (len < 2) return false;
	return (str[0] == '"' && str[len - 1] == '"');
}
static string removeQuotes(const string& str) {
	size_t len = str.length();
	if (len >= 2 && str.front() == '"' && str.back() == '"') {
		return str.substr(1, len - 2);
	} else {
		return str;
	}
}
static void validString(string s) {
	size_t first = s.find('"');
	if (first == string::npos) return; //no "" in string
	size_t last = s.rfind('"');
	if (first != 0 || last != s.size() - 1) {
		throw SyntaxError("invalid string: " + s);
	}
	if (s.find('"', first+1) != last) {
		throw SyntaxError("invalid string: " + s);
	}
}
static bool isRedirectionAtEnd(const vector<string> parts, string part, size_t index) {
	size_t n = parts.size();
	if (n < 2) return false;
	if (part == "<" || part == ">" || part == ">>") {
		if (index == n - 1) return false;
		if (n > 2) {
			if (index == n - 2 && parts[n - 1][0] != '>' && parts[n - 1][0] != '<') return true;
			if (n > 3) {
				if ((index == n - 3 && parts[n - 2][0] != '>' && parts[n - 2][0] != '<' && (parts[n - 1][0] == '>' || parts[n - 1][0] == '<'))
					|| (index == n - 2 && parts[n - 1][0] != '>' && parts[n - 1][0] != '<' && (parts[n - 3][0] == '>' || parts[n - 3][0] == '<'))) return true;
				if (n > 4) {
					if ((index == n - 4 || index == n - 2)
						&& parts[n - 3][0] != '>' && parts[n - 3][0] != '<'
						&& parts[n - 1][0] != '>' && parts[n - 1][0] != '<') return true;
				}
			} else throw SyntaxError("redirection incorrect");
		}
	}
	if (part.length() > 1 && (part[0] == '<' || part[0] == '>')) {
		string last = parts[n - 1];
		if (last == part) return true;
		if (n > 2) {
			if (parts[n - 2] == part) {
				return (last[0] == '<' || last[0] == '>');
			} else if (parts[n - 3] == part && (parts[n - 2] == "<" || parts[n - 2] == ">" || parts[n - 2] == ">>")) {
				return (last[0] != '<' || last[0] != '>');
			}
		}
	}

	return false;
}

CommandBuilder::CommandBuilder(Parser* p, bool pipe, bool first) : parser(p),pipe(pipe),first(first) {}

void CommandBuilder::parametrize(Command* c, vector<string>& lineparts, bool tr) {
	size_t n = lineparts.size();

	if (CommandC* cc = dynamic_cast<CommandC*>(c)) {
		if (!tr) {
			bool hasopt = false;
			bool hasarg = false;
			bool hasifile = false;
			bool hasofile = false;
			if (n == 1) {
				cc->setInstream(CONSOLE, "");
				cc->setOutstream(CONSOLE, "");
			} else {
				for (size_t i = 1; i < n; i++) {
					string part = lineparts[i];
					size_t len = part.length();
					//more than 1 opt in the future?
					if (part[0] == '-' && len > 1) {
						//if (len > 2 || (!(part[1] >= 'a' && part[1] <= 'z'))) throw SyntaxError("invalid option");
						if (len > 7) throw SyntaxError("invalid argument");
						if (lineparts[1] == part) {
							if (!hasopt) {
								cc->setOpt(part.substr(1));
								hasopt = true;
							} else throw SemanticError("option already stated");
						} else {
							throw SyntaxError("option must be at the beginning of the command");
						}
					} else if (quotesCheck(part)) {
						int quotes = count(part.begin(), part.end(), '"');
						if (quotes != 2) throw SyntaxError("invalid string: " + part);
						if (!hasarg) {
							hasarg = true;
							cc->setArg(removeQuotes(part));
							cc->setInstream(ARG, "");
						} else {
							throw SemanticError("argument already stated");
						}
					} else if (part[0] == '<') {
						if (isRedirectionAtEnd(lineparts, part, i)) {
							if (!hasifile) {
								hasifile = true;
								if (len > 1) {
									cc->setInstream(FFILE, part);
								} else {
									if (lineparts[i + 1][0] == '>' || lineparts[i + 1][0] == '<') throw SyntaxError("incorrect redirection");
									cc->setInstream(FFILE, lineparts[i + 1]);
								}
							} else {
								throw SemanticError("input redirection already stated");
							}
						} else {
							throw SyntaxError("redirection must be at the end of the command");
						}
					} else if (part[0] == '>') {
						if (isRedirectionAtEnd(lineparts, part, i)) {
							if (!hasofile) {
								hasofile = true;
								if (part == ">" || part == ">>") {
									if (lineparts[i + 1][0] == '>' || lineparts[i + 1][0] == '<') throw SyntaxError("incorrect redirection");
									cc->setOutstream(FFILE, part + lineparts[i + 1]);
								} else {
									cc->setOutstream(FFILE, part);
								}
							} else throw SemanticError("output redirection already stated");
						} else {
							throw SyntaxError("redirection must be at the end of the command");
						}
					} else {
						validString(part);
						if (i == 1) {
							if (!hasarg && !hasifile) {
								hasarg = true;
								hasifile = true;
								cc->setInstream(FFILE, part);
							} else throw SemanticError("argument already stated");
						} else if (i > 1) {
							if (i == 2 && lineparts[1][0] == '-') {
								if (!hasarg && !hasifile) {
									hasarg = true;
									hasifile = true;
									cc->setInstream(FFILE, part);
								} else throw SemanticError("argument already stated");
							} else if (lineparts[i - 1][0] != '<' && lineparts[i - 1][0] != '>') throw SemanticError("argument already stated");
						}
					}
				}
			}
		} else {
			Tr* t = dynamic_cast<Tr*>(cc);

			if (n == 1) throw SyntaxError("too few arguments");
			if (n > 7) throw SyntaxError("too many arguments");
			int quotes = 0, file = 0, inredirect = 0, outredirect = 0;
			for (size_t i = 1; i < n; i++) {
				string part = lineparts[i];
				validString(part);
				if (quotesCheck(part)) quotes++;
				if (part[0] == '<') inredirect++;
				if (part[0] == '>') outredirect++;
				if (!quotesCheck(part) && part[0] != '>' && part[0] != '<') {
					if (i > 1 && lineparts[i-1][0]!='<' && lineparts[i-1][0]!='>') {
						throw SyntaxError("input file must be the first argument");
					}
					file++;
				}
			}
			if (quotes == 0) throw SyntaxError("too few arguments - 'what' not stated");
			if (quotes > 3) throw SyntaxError("too many arguments");
			if (inredirect > 1 || outredirect > 1) throw SemanticError("redirection already stated");
			if ((file == 1 && quotes > 2) || (file == 1 && inredirect == 1) || (quotes > 2 && inredirect == 1)) throw SemanticError("input stream already stated");
			if (file > 1) {
				if ((file == 2 && inredirect != 1 && outredirect != 1)
					|| (file == 3 && !(inredirect == 1 && outredirect == 1))
					|| file > 3) {
					throw SemanticError("input file already stated");
				}
			}

			for (size_t i = 0; i < n; i++) {
				string part = lineparts[i];
				if (outredirect == 1 && part[0] == '>') {
					if (isRedirectionAtEnd(lineparts, part, i)) {
						if (part == ">" || part == ">>") {
							if (lineparts[i + 1][0] == '>' || lineparts[i + 1][0] == '<') throw SyntaxError("incorrect redirection");
							cc->setOutstream(FFILE, part + lineparts[i + 1]);
						} else {
							cc->setOutstream(FFILE, part);
						}
					} else throw SyntaxError("redirection must be at the end of the command");
				}
			}

			if (n > 2) {
				if (!quotesCheck(lineparts[1]) && lineparts[1][0] != '<' && lineparts[1][0] != '>') {
					if (pipe && !first) throw SemanticError("input stream can be stated only in the first command in a pipe");
					t->setInstream(FFILE, lineparts[1]);
				} else {
					for (size_t i = 0; i < n; i++) {
						string part = lineparts[i];
						if (inredirect == 1 && part[0] == '<') {
							if (isRedirectionAtEnd(lineparts, part, i)) {
								if (part.length() > 1) {
									cc->setInstream(FFILE, part);
								} else {
									if (lineparts[i + 1][0] == '>' || lineparts[i + 1][0] == '<') throw SyntaxError("incorrect redirection");
									cc->setInstream(FFILE, lineparts[i + 1]);
								}
								t->setArg("");
							} else throw SyntaxError("redirection must be at the end of the command");
						}
					}
					if (t->getInstream() == CONSOLE && quotes > 1) {
						t->setInstream(ARG, "");
						t->setArg(removeQuotes(lineparts[1]));
					}
				}
			} else if (n == 2) {
				t->setInstream(CONSOLE, "");
				t->setOutstream(CONSOLE, "");
				t->setWhat(removeQuotes(lineparts[1]));
				t->setWith("");
				return;
			}

			if (quotes == 1) {
				if (!quotesCheck(lineparts[1])) {
					t->setWhat(removeQuotes(lineparts[2]));
					t->setWith("");
				} else {
					t->setWhat(removeQuotes(lineparts[1]));
					t->setWith("");
				}
			} else if (quotes == 2) {
				if (pipe) {
					if (!first) {
						t->setInstream(PIPE, "");
						t->setWhat(removeQuotes(lineparts[1]));
						t->setWith(removeQuotes(lineparts[2]));
					} else {
						t->setWhat(removeQuotes(lineparts[2]));
						t->setWith("");
					}
				} else {
					if (inredirect == 1) {
						t->setWhat(removeQuotes(lineparts[1]));
						t->setWith(removeQuotes(lineparts[2]));
					} else if (!quotesCheck(lineparts[1])) {
						t->setWhat(removeQuotes(lineparts[2]));
						t->setWith(removeQuotes(lineparts[3]));
					} else if (quotesCheck(lineparts[1])) {
						t->setWhat(removeQuotes(lineparts[2]));
						t->setWith("");
					}
				}
			} else if (quotes == 3) {
				if (pipe) {
					if (!first) throw SemanticError("only first command in a pipe can have input stream");
				}
				t->setWhat(removeQuotes(lineparts[2]));
				t->setWith(removeQuotes(lineparts[3]));
			}
		}
	}
}
Command* CommandBuilder::buildCommand(vector<string>& lineparts) {
    Command* c = nullptr;
	size_t n = lineparts.size();

	bool tr = false;
	if (lineparts[0] == "echo") {
		c = new Echo();
	} else if (lineparts[0] == "exit") {
		if (n > 1) {
			throw SyntaxError("command 'exit' doesn't have arguments");
		}
		c = new Exit();
	} else if (lineparts[0] == "prompt") {
		if (n > 2) {
			throw SyntaxError("command 'prompt' takes only 1 argument");
		} else if (n == 1) {
			throw SyntaxError("argument expected");
		}
		c = new Prompt(removeQuotes(lineparts[1]));
	} else if (lineparts[0] == "time") {
		if (n > 1) {
			if (lineparts[1][0] != '>') {
				throw SyntaxError("command 'time' doesn't have input stream");
			}
		}
		c = new Time();
	} else if (lineparts[0] == "date") {
		if (n > 1) {
			if (lineparts[1][0] != '>') {
				throw SyntaxError("command 'date' doesn't have input stream");
			}
		}
		c = new Date();
	} else if (lineparts[0] == "touch") {
		if (n > 2) {
			throw SyntaxError("command 'touch' takes only 1 argument");
		} else if (n == 1) {
			throw SyntaxError("argument expected");
		}
		c = new Touch(lineparts[1]);
	} else if (lineparts[0] == "truncate") {
		if (n > 2) {
			throw SyntaxError("command 'truncate' takes only 1 argument");
		} else if (n == 1) {
			throw SyntaxError("argument expected");
		}
		c = new Truncate(lineparts[1]);
	} else if (lineparts[0] == "rm") {
		if (n > 2) {
			throw SyntaxError("command 'rm' takes only 1 argument");
		} else if (n == 1) {
			throw SyntaxError("argument expected");
		}
		c = new Rm(lineparts[1]);
	} else if (lineparts[0] == "wc") {
		if (n < 2) {
			throw SyntaxError("option must be stated");
		}
		c = new Wc();
	} else if (lineparts[0] == "tr") {
		c = new Tr();
		tr = true;
	} else if (lineparts[0] == "head") {
		if (n < 2) {
			throw SyntaxError("option must be stated");
		}
		c = new Head();
	} else if (lineparts[0] == "batch") {
		/*if (n > 1) {
			if (lineparts[1][0] == '>') {
				throw SyntaxError("command 'batch' doesn't have output stream");   //actually not true
			}
		}*/
		c = new Batch(*parser);
	} else {
		throw UnknownCommandException(lineparts[0]);
	}

	parametrize(c, lineparts, tr);

    return c;
}
