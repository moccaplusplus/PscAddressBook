#pragma once

#include <string>
#include <vector>

using namespace std;

/**
	Ascetic menu.
	Just prints key, label pairs and tries to adjust key (or label) to user input string.
*/
class Menu {
private:
	/**
		Why not a map - to keep insertion order.
	*/
	typedef vector<pair<string, string>> Entries;

	Entries entries;
	Entries::iterator checkKey(const string& key);
public:
	Menu();
	Menu(const Entries& entries);

	/**
		Just print list of choices.
	*/
	void print();

	/**
		Prompt user-user input, and return matching menu entry if any.
	*/
	string getInput();
};
