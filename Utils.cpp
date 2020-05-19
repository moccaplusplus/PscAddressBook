#include <algorithm>
#include <cstdio>
#include <stdexcept>
#include "Utils.h"

vector<string> Utils::split(const string& s, const string& delimiter) {
	vector<string> result;
	size_t prev = 0;
	auto pos = s.find(delimiter);
	while (pos != string::npos) {
		if (pos > prev && s.at(pos - 1) == '\\') continue; // escaped
		result.push_back(s.substr(prev, pos - prev));
		prev = pos + 1;
		pos = s.find(delimiter, prev);
	}
	result.push_back(s.substr(prev, pos - prev));
	return result;
}

string Utils::join(const vector<string>& v, const string& delimiter) {
	if (v.empty()) return "";
	string result = v[0];
	for (auto it = v.begin() + 1; it != v.end(); it++) result += delimiter + (*it);
	return result;
}

string Utils::replaceAll(const string& s, const string& seq, const string& repl) {
	size_t prev = 0;
	string result = "";
	auto pos = s.find(seq);
	while (pos != string::npos) {
		result += (s.substr(prev, pos - prev)) + repl;
		prev = pos + 1;
		pos = s.find(seq, prev);
	}
	result += s.substr(prev, pos - prev);
	return result;
}

string Utils::ellipsis(const string& s, unsigned maxLength) {
	return s.size() <= maxLength ? s : s.substr(0, maxLength - 2) + "..";
}

string Utils::escape(const string& s, const string& seq) {
	return replaceAll(s, seq, "\\" + seq);
}

string Utils::unescape(const string& s, const string& seq) {
	return replaceAll(s, "\\" + seq, seq);
}

vector<string> Utils::escapeAll(const vector<string>& v, const string& seq) {
	vector<string> copy;
	transform(v.begin(), v.end(), back_inserter(copy), [&seq](auto s) { return escape(s, seq); });
	return copy;
}

vector<string> Utils::unescapeAll(const vector<string>& v, const string& seq) {
	vector<string> copy;
	transform(v.begin(), v.end(), back_inserter(copy), [&seq](auto s) { return unescape(s, seq); });
	return copy;
}

string Utils::toLowerCase(const string& s) {
	string copy;
	transform(s.begin(), s.end(), back_inserter(copy), tolower);
	return copy;
}

bool Utils::compareCaseInsensitive(const string& s1, const string& s2) {
	return toLowerCase(s1) < toLowerCase(s2);
}

bool Utils::contains(const string& haystack, const string& needle) {
	return haystack.find(needle) != string::npos;
}

bool Utils::containsCaseInsensitive(const string& haystack, const string& needle) {
	return contains(toLowerCase(haystack), toLowerCase(needle));
}

string Utils::inputString() {
	static const unsigned buffSize = 1024;
	char buffer[buffSize] = { '\0' };
	printf("> ");
	return gets_s(buffer, buffSize - 1) == nullptr ? "" : buffer;
}

string Utils::inputNonEmptyString() {
	string s = inputString();
	while (s.empty()) {
		printf("Should not be empty");
		s = inputString();
	}
	return s;
}

bool Utils::inputInt(int& result) {
	string s = inputString();
	try {
		result = stoi(s);
		return true;
	}
	catch (invalid_argument e) {
		return false;
	}
}

int Utils::inputInt() {
	int result;
	while (!inputInt(result)) printf("Not a number");
	return result;
}

int Utils::inputInt(int fromIncl, int toExcl) {
	int n = inputInt();
	while (n < fromIncl || n >= toExcl) {
		printf("Out of range");
		n = inputInt();
	}
	return n;
}

int Utils::inputInt(int fromIncl, int toExcl, int defaultValue) {
	int n;
	string s;
	while (true) {
		s = inputString();
		if (s.empty()) return defaultValue;
		try {
			n = stoi(s);
			if (n >= fromIncl && n < toExcl) return n;
			printf("Out of range");
		}
		catch (invalid_argument e) {
			printf("Not a number");
		}
	}
}

bool Utils::inputYesOrNo(bool defaultsYes) {
	printf(defaultsYes ? "[y or empty/N]" : "[y/N or empty]");
	const auto input = toLowerCase(inputString());
	return defaultsYes ? "n" != input : "y" == input;
}

Date Utils::inputDate() {
	while (true) {
		string s = inputNonEmptyString();
		Date d;
		if (sscanf_s(s.c_str(), "%4d-%2d-%2d", &d.year, &d.month, &d.day) == 3) return d;
		else printf("Invalid date format");
	}
}

Date Utils::inputDate(const Date& from, const Date& to) {
	while (true) {
		Date d = inputDate();
		if (d.pseudotime() >= from.pseudotime() && d.pseudotime() <= to.pseudotime()) return d;
		else printf("Invalid date range");
	}
}

bool Utils::inputDateOptional(Date& target) {
	while (true) {
		string s = inputString();
		if (s.empty()) return false;
		Date d;
		if (sscanf_s(s.c_str(), "%4d-%2d-%2d", &d.year, &d.month, &d.day) == 3) {
			target = d;
			return true;
		}
		printf("Invalid date format");
	}
}

bool Utils::inputDateOptional(Date& target, const Date& from, const Date& to) {
	while (true) {
		Date d;
		if (!inputDateOptional(d)) return false;
		if (d.pseudotime() >= from.pseudotime() && d.pseudotime() <= to.pseudotime()) {
			target = d;
			return true;
		}
		printf("Invalid date range");
	}
}

void Utils::printTable(vector<Contact>::const_iterator it, const vector<Contact>::const_iterator& end) {
	printf("%3s  %15s  %15s  %12s  %15s  %20s  %20s\n",
		"Id", "First Name", "Surname", "Birth Date", "Main Phone", "Main Email", "Address");
	printf("-----------------------------------------------------------------------------------------------------------------\n");
	for (; it != end; it++)
		printf(
			"%3d  %15.15s  %15.15s  %12.12s  %15.15s  %20.20s  %20.20s\n",
			it->id,
			ellipsis(it->firstname, 15).c_str(),
			ellipsis(it->surname, 15).c_str(),
			it->birthdate.toString().c_str(),
			ellipsis(it->getMainPhone(), 15).c_str(),
			ellipsis(it->getMainEmail(), 20).c_str(),
			ellipsis(it->address, 20).c_str());
}
