#include "headers/pch.h"

#include "headers/exceptions/AccessViolationException.h"


using namespace CustomExceptionsNS;

void AccessViolationException::setWhat(std::string w) {
	this->_what = w;
}

std::string AccessViolationException::what() {
	return this->_what;
}
