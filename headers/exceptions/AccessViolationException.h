#pragma once

#include "headers/pch.h"

namespace CustomExceptionsNS {

	class AccessViolationException : public std::exception {
	private:
		std::string _what;

	public:
		void setWhat(std::string w);
		std::string what();
	};

}
