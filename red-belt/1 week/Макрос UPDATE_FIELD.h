#include <iostream> 
#include <algorithm> 
#include <vector> 
#include <string> 
#include <map>
#include <deque>
#include <set>
#include <iomanip>
#include <numeric>
#include <queue>
#include <sstream>
#include <cstdint>
#include <tuple>
#include "test_runner.h"
#include "airline_ticket.h"
using namespace std;

bool operator<(const Time& lhs, const Time& rhs) {
	if (lhs.hours != rhs.hours)
		return lhs.hours < rhs.hours;
	else
		return lhs.minutes < lhs.minutes;
}
bool operator==(const Time& lhs, const Time& rhs) {
	return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes;
}



bool operator<(const Date& lhs, const Date& rhs) {
	if (lhs.year != rhs.year)
		return lhs.year < rhs.year;
	else if (lhs.month != rhs.month)
		return lhs.month < rhs.month;
	else
		return lhs.day < rhs.day;
}

bool operator==(const Date& lhs, const Date& rhs) {
	return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
}

istream& operator>>(istream& in, Time& time) {
	string str;
	in >> str;
	char a;
	stringstream ss(str);
	ss >> time.hours;
	ss >> a;
	ss >> time.minutes;
	return in;
}

istream& operator>>(istream& in, Date& date) {
	string str;
	in >> str;
	char a;
	stringstream ss(str);
	ss >> date.year;
	ss >> a;
	ss >> date.month;
	ss >> a;
	ss >> date.day;
	return in;
}

#define UPDATE_FIELD(ticket, field, values) { \
	map<string, string>::const_iterator it; \
	it = values.find(#field); \
	if (it != values.end()) { \
		istringstream is(it->second); \
		is >> ticket.field; \
	}\
}