#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../protocol.h"

int main() {
	std::cout << "From server\n";
	
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		std::cerr << "Error initializing socket\n";
		return 1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;		// Designates IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;	// Wildcard
	server_addr.sin_port = htons(4221); 		// Big-endian (MSB)
	
	int retval;

	retval = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (retval == -1) {
		std::cerr << "Error binding socket\n";
		return 1;
	}

	retval = listen(server_fd, 3); 	// Socket, backlog
	if (retval == -1) {
		std::cerr << "Error marking socket for connection\n";
		return 1;
	}

	while (true) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if (client_fd == -1) {
			std::cerr << "Error accepting socket connection\n";
			return 1;
		}

		uint8_t buffer[MAX_PACKET_SIZE] = {0};

		retval = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
		if (retval == -1) {
			std::cerr << "Error reading client fd\n";
			return 1;
		}

		message msg = {0};
		result res = {0};

		decode(buffer, &msg, &res);

		print_message(&msg);
	}

	return 0;
} 
