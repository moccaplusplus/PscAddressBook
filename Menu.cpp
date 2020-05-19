#include <algorithm>
#include "Menu.h"
#include "Utils.h"

using namespace Utils;

Menu::Menu() {}
Menu::Menu(const Entries& entries) : entries(entries) {}

void Menu::print() {
	vector<string> v;
	transform(entries.begin(), entries.end(), back_inserter(v), [](auto e) { return "[" + e.first + "] " + e.second; });
	printf("%s", join(v, ", ").c_str());
}

string Menu::getInput() {
	while (true) {
		string key = inputString();
		auto found = checkKey(key);
		if (found != entries.end()) return found->first;
		printf("Unknown command");
	}
}

Menu::Entries::iterator Menu::checkKey(const string& key) {
	const auto lowerCaseKey = toLowerCase(key);
	return find_if(
		entries.begin(), entries.end(),
		[&lowerCaseKey](auto e) { return toLowerCase(e.first) == lowerCaseKey || toLowerCase(e.second) == lowerCaseKey; });
}