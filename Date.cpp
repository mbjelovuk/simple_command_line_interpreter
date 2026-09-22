#include "Date.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

Date::Date() {
	name = "date";
}
void Date::execute() {
	//'date' doesn't have input stream
	process();
	outputstr();
}
void Date::process() {
	time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm local;
	localtime_s(&local, &now);
	ostringstream oss;
	oss << put_time(&local, "%d.%m.%Y.");
	string timeStr = oss.str();
	result = timeStr;
}
