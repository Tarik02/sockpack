#include <iostream>
#include <vector>
#include <thread>
#include "sockpack/Endpoint.h"
#include "sockpack/Socket.h"
#include "sockpack/ServerSocket.h"

using namespace std;
using namespace sockpack;

const unsigned short Port = 16747;
vector<thread> threads;
bool running = true;


void eachClientThread(Socket *socket) {
	cout << "Connected client" << endl;

	string message;
	size_t length;

	message.resize(sizeof(length));
	socket->receive(message);
	length = *(decltype(length) *)&message[0];
	message.resize(length);
	socket->receive(message);

	socket->send({(char *)&length, sizeof(length)});
	socket->send(message);
}

void serverThread() {
	ServerSocket serverSocket;
	serverSocket.bind(Endpoint{Port});

	while (running) {
		auto clientSocket = serverSocket.accept();
		threads.emplace_back(eachClientThread, clientSocket);
	}
}

void clientThread() {
	Socket client;

	cout << "Connecting..." << endl;
	client.connect({"127.0.0.1", Port});
	cout << "Connected, sending." << endl;

	std::string message = "Hello world";
	size_t length = message.length();
	client.send({(char *)&length, sizeof(length)});
	client.send(message);

	message.resize(sizeof(length));
	client.receive(message);
	length = *(decltype(length) *)&message[0];
	message.resize(length);
	client.receive(message);

	cout << "Received " << message << endl;
}

int main() {
	threads.emplace_back(serverThread);
	threads.emplace_back(clientThread);

	running = false;

	for (auto &thread : threads) {
		thread.join();
	}

	return 0;
}
