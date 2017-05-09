#include "sockpack/Socket.h"
#include "sockpack/Exception.h"
#include <thread>
#include <zconf.h>

namespace sockpack {
	Socket::Socket() : state(State::Empty), handle(-1) {}
	Socket::Socket(int handle) : state(State::Connected), handle(handle) {}

	void Socket::connect(const Endpoint &endpoint) {
		if (state == State::Connecting) {
			throw Exception("Already connecting.");
		}

		if ((state == State::Connected) || (state == State::Reading)) {
			throw Exception("Already connected.");
		}

		state = State::Connecting;

		if ((handle = ::socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			state = State::Empty;
			throw Exception("Failed to create a socket.");
		}

		if (::connect(handle, (struct sockaddr *)&endpoint.address, sizeof(endpoint.address)) < 0) {
			state = State::Empty;
			throw Exception("Failed to connect.");
		}

		state = State::Connected;
	}

	void Socket::close() {
		if (state != State::Empty) {
			shutdown(handle, SHUT_WR);

			if (state != State::Reading) {
				::close(handle);
			}

			state = State::Empty;
		}
	}

	void Socket::receive(std::string &buffer) {
		if ((state == State::Empty) || (state == State::Connecting)) {
			throw Exception("Don't connected.");
		}

		while (state == State::Reading) {
			std::this_thread::yield();
		}
		state = State::Reading;

		auto needed = buffer.size();
		while (needed != 0) {
			auto received = ::recv(handle, (void *)&buffer[buffer.size() - needed], needed, 0);
			if (received < 0) {
				if (state != State::Reading) {
					::close(handle);
				}

				throw Exception("Failed to receive data.");
			}

			needed -= received;
		}

		if (state == State::Reading) {
			state = State::Connected;
		} else {
			::close(handle);
		}
	}

	void Socket::receive(std::string &buffer, std::size_t count) {
		buffer.reserve(count);
		buffer.resize(count);
		receive(buffer);
	}

	void Socket::send(const std::string &buffer) {
		if ((state == State::Empty) || (state == State::Connecting)) {
			throw Exception("Don't connected.");
		}

		::send(handle, &buffer[0], buffer.size(), 0);
	}
}
