#pragma once

#include <string>
#include <netinet/in.h>

namespace sockpack {
	class Endpoint {
		friend class Socket;
		friend class ServerSocket;

	private:
		sockaddr_in address;

	public:
		Endpoint();
		Endpoint(unsigned short port);
		Endpoint(const std::string &endpoint);
		Endpoint(const std::string &addressString, unsigned short port);

		std::string toString() const;
	};
}
