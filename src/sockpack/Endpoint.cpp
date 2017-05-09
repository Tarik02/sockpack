#include "sockpack/Endpoint.h"
#include <netdb.h>
#include <cstring>
#include <regex>
#include <stdlib.h>
#include <arpa/inet.h>

namespace sockpack {
	Endpoint::Endpoint() {
		address.sin_family = AF_INET;
		address.sin_port = 0; // Endianess conversion is not needed
		address.sin_addr.s_addr = INADDR_ANY;
	}

	Endpoint::Endpoint(unsigned short port) {
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = INADDR_ANY;
	}

	Endpoint::Endpoint(const std::string &endpoint) {
		std::string addressString;
		unsigned short port;

		{
			static std::regex AddressRegex("(.*)\\:(\\d+)$");
			std::smatch match;

			if (std::regex_match(endpoint, match, AddressRegex)) {
				addressString = match[1];

				try {
					port = std::atoi(match[2].str().c_str());
				} catch (...) {
					port = 0;
				}
			} else {
				addressString = endpoint;
				port = 0;
			}
		}

		const hostent *ent = gethostbyname(addressString.c_str());

		if (ent == nullptr) {
			throw std::runtime_error("Given address not found");
		}

		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		memcpy((char *)&address.sin_addr.s_addr, ent->h_addr, (size_t)ent->h_length);
	}

	Endpoint::Endpoint(const std::string &addressString, unsigned short port) {
		const hostent *ent = gethostbyname(addressString.c_str());

		if (ent == nullptr) {
			throw std::runtime_error("Given address not found");
		}

		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		memcpy((char *)&address.sin_addr.s_addr, ent->h_addr, (size_t)ent->h_length);
	}

	std::string Endpoint::toString() const {
		std::stringstream ss;

		ss
			<< inet_ntoa(address.sin_addr)
			<< ":"
			<< ntohs(address.sin_port);

		return ss.str();
	}
}
