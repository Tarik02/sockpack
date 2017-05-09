#pragma once

#include "Endpoint.h"

namespace sockpack {
	class Socket {
		friend class ServerSocket;
		enum class State : unsigned char {Empty, Connecting, Connected, Reading};

	private:
		int handle;
		State state;

	public:
		Socket();

	protected:
		Socket(int handle);

	public:
		void connect(const Endpoint &endpoint);
		void close();

		void receive(std::string &buffer);
		void receive(std::string &buffer, std::size_t count);
		void send(const std::string &buffer);
	};
}
