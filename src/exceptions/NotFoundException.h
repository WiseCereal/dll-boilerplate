#pragma once

#include "src/pch/pch.h"

namespace CustomExceptionsNS {

	class NotFoundException : public std::exception {
	private:
		std::string _what;

	public:
		void setWhat(std::string w);
		std::string what();
	};

}
