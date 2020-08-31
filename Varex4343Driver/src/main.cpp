#include "server.h"
#include "handle.h"
#include <vector>
#include <thread>

int main() {
	SOCKET entry_socket = start_server("127.0.0.1", "3490");

	while (true) {
		SOCKET client_socket = accept_client(entry_socket);
		std::thread(handle_connection, client_socket).detach();
	}
}