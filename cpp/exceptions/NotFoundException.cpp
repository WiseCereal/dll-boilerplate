#include "headers/pch.h"

#include "headers/exceptions/NotFoundException.h"


using namespace CustomExceptionsNS;

void NotFoundException::setWhat(std::string w) {
	this->_what = w;
}

std::string NotFoundException::what() {
	return this->_what;
}
