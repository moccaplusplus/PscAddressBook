#include "AddressBook.h"

using namespace std;

/**
    Entry point to the application.
*/
int main(int argc, char** argv) {
    AddressBook(argc > 1 ? argv[2] : "./contacts.txt").run();
    return EXIT_SUCCESS;
}
