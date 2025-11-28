#include "src/pch/pch.h"

#include "src/exceptions/AccessViolationException.h"


using namespace CustomExceptionsNS;

void AccessViolationException::setWhat(std::string w) {
	this->_what = w;
}

std::string AccessViolationException::what() {
	return this->_what;
}
