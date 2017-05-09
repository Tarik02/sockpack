#pragma once

#include "Endpoint.h"
#include "Socket.h"

namespace sockpack {
	class ServerSocket {
		enum class State : unsigned char {Empty, Binding, Accepting, Idle};

	private:
		int handle;
		State state;

	public:
		ServerSocket();

		void bind(const Endpoint &endpoint);
		Socket *accept();

		void close();
	};
}
