#include <cstdio>
#include "Date.h"

Date::Date() : Date(0, 0, 0) {}
Date::Date(long pseudotime) : Date(pseudotime / 10000, (pseudotime / 100) % 100, pseudotime % 100) {}
Date::Date(int year, int month, int day) : year(year), month(month), day(day) {}
Date::Date(const Date& source) : Date(source.year, source.month, source.day) {}

long Date::pseudotime() const {	
	return day + (100 * month) + (10000 * year); 
}

string Date::toString() const {
	char buff[11];
	sprintf_s(buff, "%04d-%02d-%02d", year, month, day);
	return buff;
}

bool Date::operator<(const Date& date) const { 
	return pseudotime() < date.pseudotime(); 
}
