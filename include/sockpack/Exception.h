#pragma once

#include <stdexcept>

namespace sockpack {
	class Exception : public std::runtime_error {
	public:
		inline Exception(const char *message) : std::runtime_error(message) {}
		inline Exception(const std::string &message) : std::runtime_error(message) {}
	};
}
