#include "File.h"
#include "Utils.h"

using namespace Utils;

const string File::COLUMN_SEP = ";";
const string File::LIST_SEP  = ",";

File::File(string path) : path(path) {}

void File::read(vector<Contact>& target) {
	FILE* fh;
	fopen_s(&fh, path.c_str(), "r");
	if (fh != nullptr) {
		readAll(fh, target);
		fclose(fh);
	}
}

void File::write(const vector<Contact>& source) {
	if (source.empty()) return;
	FILE* fh;
	fopen_s(&fh, path.c_str(), "w");
	if (fh != nullptr) {
		writeAll(fh, source);
		fclose(fh);
	}
}

void File::readAll(FILE* fh, vector<Contact>& target) {
	static const unsigned buffSize = 8 * 1024;
	char lineBuffer[buffSize];
	while (fgets(lineBuffer, 8 * 1024, fh) != nullptr) {
		const auto length = strlen(lineBuffer);
		if (length > 0 && lineBuffer[length - 1] == '\n') lineBuffer[length - 1] = '\0';
		Contact record;
		readOne(lineBuffer, record);
		record.id = target.size() + 1;
		target.push_back(record);
	}
}

void File::readOne(const string& line, Contact& target) {
	vector<string> columns = unescapeAll(split(line, COLUMN_SEP), COLUMN_SEP);
	target.firstname = columns[0];
	target.surname = columns[1];
	target.address = columns[2];
	target.birthdate = columns[3] == "" ? 0 : stol(columns[3]);
	target.phones = unescapeAll(split(columns[4], LIST_SEP), LIST_SEP);
	target.emails = unescapeAll(split(columns[5], LIST_SEP), LIST_SEP);
}

void File::writeAll(FILE* out, const vector<Contact>& source) {
	for (auto& row : source) writeOne(out, row);
}

void File::writeOne(FILE* out, const Contact& source) {
	fprintf(out, "%s\n", join(escapeAll({
			source.firstname,
			source.surname,
			source.address,
			to_string(source.birthdate.pseudotime()),
			join(escapeAll(source.phones, LIST_SEP), LIST_SEP),
			join(escapeAll(source.emails, LIST_SEP), LIST_SEP)
		}, COLUMN_SEP), COLUMN_SEP).c_str());
}
