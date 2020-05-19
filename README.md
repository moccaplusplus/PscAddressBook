# PscAddressBook

Entry point: `main.cpp`.

From there the app structure can be followed by includes and calls.

Project is build with Microsoft Visual Studio Community 2019.

It is very simple - especially user interaction is simplified - but works reliably.

Header files are documented. The sourrrce code should also look informative 
as I tried to provide names self-explanatory as much as possible.

I used `cstdio` over `iostream` intentionally.

Data file format is as follows:
- every line corresponds to one contact.
- the column delimiter is `;`
- some columns (e.g. email and phone) support mulitple entries - these in turn are delimited by `,`.
- example data file is also in the repo under name `contacts.txt`.
