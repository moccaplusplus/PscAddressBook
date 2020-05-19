#pragma once
#include <cstdio>
#include <string>
#include <vector>
#include "Contact.h"

using namespace std;

/**
	Utility class for writing contact list to file.
	Data format is as follows:
		- every line corresponds to one contact.
		- the column delimiter is `;`
		- some columns (e.g. email and phone) support mulitple entries - these in turn are delimited by `,`.

*/
class File {
public:

	static const string COLUMN_SEP;
	static const string LIST_SEP;

	/**
		The path to actual filesystem file.
	*/
	const string path;

	File(string path);

	/** 
		Reads file contents (given by path in constructor) into the vector passed as argument.
	*/
	void read(vector<Contact>& target);

	/**
		Reads the vector passed as argument into the file associated with this object (e.g. passed as constructor argument).
	*/
	void write(const vector<Contact>& source);
private:
	void readAll(FILE* in, vector<Contact>& target);
	void readOne(const string& in, Contact& target);
	void writeAll(FILE* out, const vector<Contact>& source);
	void writeOne(FILE* out, const Contact& source);
};

