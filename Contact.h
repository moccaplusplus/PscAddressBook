#pragma once

#include <string>
#include <vector>
#include "Date.h"

using namespace std;

/**
	Simple entity-DTO-like object for storing and passing by contact information.
	Everything is public and mutable.
	Field names are generally self descriptive.
*/
class Contact {
public:
	/**
		Compares two contact by first-name (in lexical order).
	*/
	static bool compareByFirstName(const Contact& c1, const Contact& c2);
	/**
		Compares two contact by surname (in lexical order).
	*/
	static bool compareBySurname(const Contact& c1, const Contact& c2);
	/**
		Compares two contact by birthdate.
	*/
	static bool compareByBirthdate(const Contact& c1, const Contact& c2);

	unsigned id;
	string firstname;
	string surname;
	string address;
	Date birthdate;
	vector<string> emails;
	vector<string> phones;

	/**
		Returns first email if exists, empty string otherwise.
	*/
	string getMainEmail() const;
	/**
		Returns first phone if exists, empty string otherwise.
	*/
	string getMainPhone() const;
	/**
		Tests if this contact contains text (used for text occuerence search).
	*/
	bool containsText(string) const;
};
