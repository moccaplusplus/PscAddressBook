#pragma once

#include <functional>
#include "Contact.h"
#include "File.h"
#include "Menu.h"

using namespace std;

/** 
	The core class - where all the magic happens ;).
*/
class AddressBook {
public:
	/**
		Creates instance - with path to data file.
	*/
	AddressBook(string path);
	~AddressBook();

	/**
		The main app loop.
		Al it does is to check if action field.
		if it contains a valid pointer - execute it, otherwise exit application.

		See action field below.
	*/
	void run();

private:
	typedef function<bool(const Contact&, const Contact&)> Compare;
	typedef function<bool(const Contact&)> Filter;

	/**
		File read/write helper.

		See class File in File.h.
	*/
	File file;

	/** 
		Predefined menus.
	*/
	Menu mainMenu;
	Menu filterMenu;
	Menu sortMenu;
	Menu chunkMenu;

	/**
		Actual data.
	*/
	vector<Contact> contacts;

	/**
		Data copy - for actions like filtering and sorting which narrow displayed dataset.
	*/
	vector<Contact> copy;

	/**
		Helper flags for paging and fitering.
	*/
	bool useCopy;
	unsigned chunkOffset;
	unsigned chunkSize;

	/**
		It is a pointer to "next action",
		where action is one of void *Sceeen() methods
		(for example void listScreen() - see below).

		the action field is intiilised with pointer to menuScreen.
		then each action method should set this field to hold pointer
		to the next action. So it is responsibility of the action method 
		to ensure a proper successor method.

		See also run method (above).
	*/
	void (AddressBook::* action)();

	/**
		The "action" methods. 
		Generally methods names are self-descriptive.

		See also: action, run().
	*/
	void menuScreen();
	void listScreen();
	void filterScreen();
	void sortScreen();
	void addScreen();
	void detailScreen();
	void editScreen();
	void removeScreen();

	/**
		private helper methods used by actions.
	*/
	unsigned selectIndex();
	Contact& selectContact();
	void textFilter();
	void dateFilter();
	void applyFilter(Filter);
	void inputList(vector<string>& target, const string& name);
	void updateList(vector<string>& target, const string& name);
};
