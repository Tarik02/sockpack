#include "sockpack/ServerSocket.h"
#include "sockpack/Exception.h"
#include <zconf.h>

namespace sockpack {
	ServerSocket::ServerSocket() : state(State::Empty) {}

	void ServerSocket::bind(const Endpoint &endpoint) {
		if (state != State::Empty) {
			throw Exception("Socket is not empty.");
		}

		state = State::Binding;
		if ((handle = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			close();
			throw Exception("Failed to create a socket.");
		}

		int optval = 1;
		if (::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0) {
			close();
			throw Exception("Failed to set socket option.");
		}

		if (::bind(handle, (struct sockaddr *)&endpoint.address, sizeof(endpoint.address)) < 0) {
			close();
			throw Exception("Failed to bind a socket.");
		}

		if (::listen(handle, 0) < 0) {
			close();
			throw Exception("Failed to bind a socket.");
		}

		state = State::Idle;
	}

	void ServerSocket::close() {
		if (state == State::Empty) {
			throw Exception("Socket is already closed.");
		}

		::close(handle);

		state = State::Empty;
	}

	Socket *ServerSocket::accept() {
		if (state != State::Idle) {
			throw Exception("Socket is not idle.");
		}
		state = State::Accepting;

		struct sockaddr_in address;
		auto sizeofaddress = sizeof(address);
		int clientSocket = ::accept(handle, (struct sockaddr *)&address, (socklen_t *)&sizeofaddress);
		if (clientSocket < 0) {
			state = State::Idle;
			throw Exception("Cannot accept a client socket.");
		}

		state = State::Idle;
		return new Socket(clientSocket);
	}
}
