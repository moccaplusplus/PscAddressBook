#pragma once

#include<string>

using namespace std;

/**
	Simple entity-DTO-like object for simplified date abtraction.
	Everything (I mean properties) is public and mutable.
	Field names are generally self descriptive.
*/
class Date {
public:
	int year;
	int month;
	int day;

	Date();
	Date(long pseudoTime);
	Date(int year, int month, int day);
	Date(const Date& source);

	/**
		Returns pseudo-timestamps - a long value usuful for comparison.
	*/
	long pseudotime() const;

	/**
		Returns human-readable string representation.
	*/
	string toString() const;

	/**
		I know, operator overloading is generally a bad practice - but I just wanted to show it in use ;).
	*/
	bool operator<(const Date& date) const;
};
