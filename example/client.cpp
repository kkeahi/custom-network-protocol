#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../protocol.h"

int main() {
	std::cout << "From client\n";

	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd == -1) {
		std::cerr << "Error initializing client socket\n";
		return 1;
	}

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(4221);
	server_addr.sin_addr.s_addr = INADDR_ANY; // Might be an issue 

	socklen_t server_addr_len = sizeof(server_addr);

	int retval;
	
	retval = connect(client_fd, (struct sockaddr *)&server_addr, server_addr_len);
	if (retval == -1) {
		std::cerr << "Error connecting from client to server socket\n";
		return 1;
	}
	
	message msg {
		VERSION_1,
		TYPE_DATA,
		2,
		{"test1\0", "test2\0"}
	};
	print_message(&msg);

	result res {0};

	uint8_t buffer[MAX_PACKET_SIZE] = {0};

	encode(buffer, &msg, &res);

	std::cout << "res.size: " << res.size << '\n';
	std::cout << "res.response: " << (int)res.response << '\n';

	retval = send(client_fd, buffer, sizeof(buffer), 0);
	if (retval == -1) {
		std::cerr << "Error sending packets from client\n";
		return 1;
	}
	return 0;
}
