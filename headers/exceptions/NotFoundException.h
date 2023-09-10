#pragma once

#include "headers/pch.h"

namespace CustomExceptionsNS {

	class NotFoundException : public std::exception {
	private:
		std::string _what;

	public:
		void setWhat(std::string w);
		std::string what();
	};

}
