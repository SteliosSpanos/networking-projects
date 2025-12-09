#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>

void handle_request(int client_socket);

int main() {

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		perror("socket:");
		exit(1);
	}

	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	if (bind_status == -1) {
		perror("bind:");
		exit(1);
	}

	int listen_status = listen(server_socket, 5);
	if (listen_status == -1) {
		perror("listen:");
		exit(1);
	}

	while(1) {
		int client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == -1) {
			perror("accept:");
			continue;
		}

		handle_request(client_socket);
		close(client_socket);
	}


	return 0;
}

void handle_request(int client_socket) {
	char buffer[4096];
	int recv_status = recv(client_socket, buffer, sizeof(buffer));
	if (recv_status == -1) {
		perror("recv:");
		exit(1);
	}
	if (recv_status == 0) {
		printf("Client disconnected\n");
		return;
	}
	buffer[recv_status] = '\0';

	char method[16];
	char path[256];
	char version[16];

	sscanf(buffer, "%15s %255s %15s", method, path, version);
	printf("Method: %s, Path: %s, Version: %s\n", method, path, version);

	if (strcmp(method "GET") != 0) {
		send_error(client_socket, 405, "Method Not Allowed");
		return;
	}
}

