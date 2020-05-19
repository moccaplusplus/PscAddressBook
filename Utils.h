#pragma once

#include <string>
#include <vector>
#include "Date.h"
#include "Contact.h"

using namespace std;

/**
	Utilities for dirty work like splitting string, escaping delimeter chcaracters, reading user input...
	Function names are generally self-descriptive.
*/
namespace Utils {

	vector<string> split(const string& s, const string& delimiter);
	string join(const vector<string>& v, const string& delimiter);
	string replaceAll(const string& s, const string& seq, const string& repl);
	string ellipsis(const string& s1, unsigned maxLength);
	string escape(const string& v, const string& seq);
	string unescape(const string& v, const string& seq);
	vector<string> escapeAll(const vector<string>& s, const string& seq);
	vector<string> unescapeAll(const vector<string>& s, const string& seq);
	bool contains(const string& haystack, const string& needle);
	bool containsCaseInsensitive(const string& haystack, const string& needle);
	string toLowerCase(const string& s);
	bool compareCaseInsensitive(const string& s1, const string& s2);

	string inputString();
	string inputNonEmptyString();
	bool inputInt(int& result);
	int inputInt();
	int inputInt(int fromIncl, int toExcl);
	int inputInt(int fromIncl, int toExcl, int defaultValue);
	bool inputYesOrNo(bool defaultsYes = false);
	Date inputDate();
	Date inputDate(const Date& from, const Date& to);
	bool inputDateOptional(Date& target);
	bool inputDateOptional(Date& target, const Date& from, const Date& to);

	void printTable(vector<Contact>::const_iterator begin, const vector<Contact>::const_iterator& end);
}
