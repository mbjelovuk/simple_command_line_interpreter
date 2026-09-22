#include "Time.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

Time::Time() {
	name = "time";
}
void Time::execute() {
	//'time' doesn't have input stream
	process();
	outputstr();
}
void Time::process() {
	time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm local;
	localtime_s(&local, &now);
	ostringstream oss;
	oss << put_time(&local, "%H:%M:%S");
	string timeStr = oss.str();
	result = timeStr;
}
