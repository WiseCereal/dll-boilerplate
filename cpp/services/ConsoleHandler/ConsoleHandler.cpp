#include "headers/pch.h"

#include "headers/services/ConsoleHandler.h"

using namespace ConsoleHandlerNS;

Handler::Handler() {}

BOOL Handler::OpenConsole() {
	UINT maxTries = 20;
	UINT delayBetweenTriesInMs = 2000;
	UINT i = 0;
	while (i < maxTries) {
		BOOL r = AllocConsole();
		if (r) {
			break;
		}
		i++;
		Sleep(delayBetweenTriesInMs);
	}

	if (i >= maxTries) {
		throw std::runtime_error("Couldn't allocate the console.");
	}

	auto err = freopen_s(&this->file, "CONOUT$", "w", stdout);
	if (err) {
		throw std::runtime_error("Couldn't open the file after allocating console.");
	}

	return TRUE;
}


BOOL Handler::CloseConsole() {
	if (this->file) {
		auto err = fclose(this->file);
		if (err) {
			throw std::runtime_error("Couldn't close the console file.");
		}
	}
	if (!FreeConsole()) {
		throw std::runtime_error("Couldn't free the console.");
	}
	this->file = NULL;
	return TRUE;
}