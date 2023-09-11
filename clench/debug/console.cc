#include "console.h"

#include <iostream>

std::sig_atomic_t locked = false;

void clench::debug::debugConsoleHandler(int signal) {
#ifdef _DEBUG
	if (locked)
		return;
	locked = true;

	while (true) {
		std::string input;
		printf("DBG>");
		std::getline(std::cin, input);
		if (!input.length())
			break;
	}

	locked = false;
#endif
}
