#include "Contact.h"
#include "Utils.h"

using namespace Utils;

string Contact::getMainEmail() const {
	return emails.empty() ? "" : emails[0];
}

string Contact::getMainPhone() const {
	return phones.empty() ? "" : phones[0];
}

bool Contact::containsText(string text) const {
	if (containsCaseInsensitive(firstname, text)) return true;
	if (containsCaseInsensitive(surname, text)) return true;
	if (containsCaseInsensitive(address, text)) return true;
	for (const auto& email : emails) if (containsCaseInsensitive(email, text)) return true;
	for (const auto& phone : phones) if (containsCaseInsensitive(phone, text)) return true;
	return false;
}

bool Contact::compareByFirstName(const Contact& c1, const Contact& c2) {
	return compareCaseInsensitive(c1.firstname, c2.firstname);
}

bool Contact::compareBySurname(const Contact& c1, const Contact& c2) {
	return compareCaseInsensitive(c1.surname, c2.surname);
}

bool Contact::compareByBirthdate(const Contact& c1, const Contact& c2) {
	return c1.birthdate < c2.birthdate;
}

