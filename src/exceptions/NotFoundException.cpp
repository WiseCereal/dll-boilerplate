#include "src/pch/pch.h"

#include "src/exceptions/NotFoundException.h"


using namespace CustomExceptionsNS;

void NotFoundException::setWhat(std::string w) {
	this->_what = w;
}

std::string NotFoundException::what() {
	return this->_what;
}
