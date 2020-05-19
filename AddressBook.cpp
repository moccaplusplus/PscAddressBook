#include <algorithm>
#include <cstdio>
#include "AddressBook.h"
#include "Utils.h"

using namespace Utils;

AddressBook::AddressBook(string path) :
	action(&AddressBook::menuScreen),
	file(path),
	useCopy(false),
	chunkOffset(0),
	chunkSize(10),
	mainMenu({ {"L", "List" }, {"S", "Sort" }, {"F", "Filter" }, {"A", "Add" }, {"D", "Details" }, {"E", "Edit" }, {"R", "Remove" }, {"X", "Exit" } }),
	filterMenu({ { "T", "Text occurence" }, { "D", "Date ranges" }, { "M", "Return to Main Menu" } }),
	sortMenu({ { "F", "First Name" }, { "S", "Surname"}, { "B", "Birthdate" }, { "M", "Return to Main Menu" } }),
	chunkMenu({ {"N", "Next Page"}, { "M", "Main Menu" } }) {
	file.read(contacts);
}

AddressBook::~AddressBook() {
	file.write(contacts);
}

void AddressBook::run() {
	printf("Address Book\n");
	while (action != nullptr) (this->*action)();
}

void AddressBook::menuScreen() {
	mainMenu.print();
	const auto cmd = mainMenu.getInput();
	action = cmd == "L" ? &AddressBook::listScreen
		: cmd == "S" ? &AddressBook::sortScreen
		: cmd == "F" ? &AddressBook::filterScreen
		: cmd == "A" ? &AddressBook::addScreen
		: cmd == "D" ? &AddressBook::detailScreen
		: cmd == "E" ? &AddressBook::editScreen
		: cmd == "R" ? &AddressBook::removeScreen
		// : cmd == "X" ? nullptr
		: nullptr;
}

void AddressBook::listScreen() {
	const auto& contactsRef = useCopy ? copy : contacts;
	const auto totalCount = contactsRef.size();
	const auto limit = min(chunkOffset + chunkSize, totalCount);

	printTable(contactsRef.begin() + chunkOffset, contactsRef.begin() + limit);
	printf("%d - %d / %d.\n", chunkOffset + 1, limit, totalCount);

	if (chunkOffset + chunkSize < totalCount) {
		chunkMenu.print();
		if ("N" == chunkMenu.getInput()) {
			chunkOffset += chunkSize;
			return;
		}
	}
	useCopy = false;
	chunkOffset = 0;
	action = &AddressBook::menuScreen;
}

void AddressBook::filterScreen() {
	printf("Filter by: ");
	filterMenu.print();
	const auto cmd = filterMenu.getInput();
	if ("T" == cmd) textFilter();
	else if ("D" == cmd) dateFilter();
	else action = &AddressBook::menuScreen;
}

void AddressBook::sortScreen() {
	printf("Sort by: ");
	sortMenu.print();
	const auto cmd = sortMenu.getInput();
	const auto compare = cmd == "F" ? Contact::compareByFirstName
		: cmd == "B" ? Contact::compareByBirthdate
		: cmd == "S" ? Contact::compareBySurname
		: nullptr;
	if (compare != nullptr) {
		copy = contacts;
		useCopy = true;
		sort(copy.begin(), copy.end(), compare);
		action = &AddressBook::listScreen;
		return;
	}
	action = &AddressBook::menuScreen;
}

void AddressBook::addScreen() {
	Contact contact;
	contact.id = contacts.size() + 1;

	printf("First name");
	contact.firstname = inputNonEmptyString();

	printf("Surname");
	contact.surname = inputNonEmptyString();

	printf("Address");
	contact.address = inputNonEmptyString();

	printf("Birthdate [YYYY-MM-DD]");
	contact.birthdate = inputDate({ 1900, 1, 1 }, { 2020, 12, 31 });

	inputList(contact.emails, "Email");
	inputList(contact.phones, "Phone");

	contacts.push_back(contact);
	printf("Contact added.\n");

	action = &AddressBook::menuScreen;
}
void AddressBook::detailScreen() {
	auto& contact = selectContact();
	printf("%15s %s\n", "First name:", contact.firstname.c_str());
	printf("%15s %s\n", "Surname:", contact.surname.c_str());
	printf("%15s %s\n", "Address:", contact.address.c_str());
	printf("%15s %s\n", "Birthdate:", contact.birthdate.toString().c_str());
	printf("%15s %s\n", "Phones:", contact.getMainPhone().c_str());
	if (contact.phones.size() > 1)
		for (auto it = contact.phones.begin() + 1; it != contact.phones.end(); it++)
			printf("%15s %s\n", "", (*it).c_str());
	printf("%15s %s\n", "Emails:", contact.getMainEmail().c_str());
	if (contact.emails.size() > 1)
		for (auto it = contact.emails.begin() + 1; it != contact.emails.end(); it++)
			printf("%15s %s\n", "", (*it).c_str());
	action = &AddressBook::menuScreen;
}

void AddressBook::editScreen() {
	auto& contact = selectContact();
	string input;

	printf("First name [empty to skip]: %s", contact.firstname.c_str());
	input = inputString();
	if (!input.empty()) contact.firstname = input;

	printf("Surname [empty to skip]: %s", contact.surname.c_str());
	input = inputString();
	if (!input.empty()) contact.surname = input;

	printf("Address [empty to skip]: %s", contact.address.c_str());
	input = inputString();
	if (!input.empty()) contact.address = input;

	printf("Birthdate [empty to skip]: %s", contact.birthdate.toString().c_str());
	inputDateOptional(contact.birthdate, { 1900, 1, 1 }, { 2020, 12, 31 });

	updateList(contact.emails, "Email");
	updateList(contact.phones, "Phone");

	action = &AddressBook::menuScreen;
}

void AddressBook::removeScreen() {
	const auto index = selectIndex();
	printf("Are You sure to remove this contact? ");
	if (inputYesOrNo(false)) {
		contacts.erase(contacts.begin() + index);
		unsigned i = 1;
		for (auto& c : contacts) c.id = i++;
		printf("Contact removed.\n");
	}
	action = &AddressBook::menuScreen;
}

unsigned AddressBook::selectIndex() {
	const auto size = contacts.size();
	printf("Which contact [number: 1 - %d]", size);
	return (unsigned)(inputInt(1, size + 1) - 1);
}

Contact& AddressBook::selectContact() {
	return contacts[selectIndex()];
}

void AddressBook::textFilter() {
	printf("Text to search");
	string text = inputString();
	if (text.empty()) action = &AddressBook::menuScreen;
	else applyFilter([&text](auto c) { return c.containsText(text); });
}

void AddressBook::dateFilter() {
	int year[2], month[2], day[2];

	printf("Year bigger or equal [1900-2020, or empty to omit]");
	year[0] = inputInt(1900, 2021, 0);
	printf("Year less or equal [1900-2020, or empty to omit]");
	year[1] = inputInt(1900, 2021, 9999);

	printf("Month bigger or equal [1-12, empty to skip]");
	month[0] = inputInt(1, 13, 1);
	printf("Month less or equal [1-12, empty to skip]");
	month[1] = inputInt(1, 13, 12);

	printf("Day bigger or equal [1-31, empty to skip]");
	day[0] = inputInt(1, 32, 1);
	printf("Day less or equal [1-31, empty to skip]");
	day[1] = inputInt(1, 32, 31);

	applyFilter([year, month, day](const Contact& c) {
		const auto d = c.birthdate;
		return d.day >= day[0] && d.day <= day[1] &&
			d.month >= month[0] && d.month <= month[1] &&
			d.year >= year[0] && d.year <= year[1];
		});
}

void AddressBook::applyFilter(Filter filter) {
	copy = {};
	copy_if(contacts.begin(), contacts.end(), back_inserter(copy), filter);
	useCopy = true;
	action = &AddressBook::listScreen;
}

void AddressBook::inputList(vector<string>& target, const string& name) {
	while (true) {
		if (!target.empty()) {
			printf("Add next %s? ", name.c_str());
			if (!inputYesOrNo(false)) break;
		}
		printf("%s %d", name.c_str(), target.size() + 1);
		target.push_back(inputNonEmptyString());
	}
}

void AddressBook::updateList(vector<string>& target, const string& name) {
	string input;
	auto it = target.begin();
	unsigned i = 1;
	while (it != target.end()) {
		if (i == 1) {
			printf("%s %d [empty to skip]: %s", name.c_str(), i, (*it).c_str());
			input = inputString();
			if (input.empty()) it++, i++;
			else *it++ = input, i++;
			continue;
		}
		printf("%s %d [empty to skip, ';' to remove]: %s", name.c_str(), i, (*it).c_str());
		input = inputString();
		if (input == ";") it = target.erase(it);
		else if (input.empty()) it++, i++;
		else *it++ = input, i++;
	}
	inputList(target, name);
}
