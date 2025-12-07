#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main() {

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		printf("Socket failed\n");
		exit(1);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(12000);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	if (bind_status == -1) {
		printf("Binding failed\n");
		exit(1);
	}

	int listen_status = listen(server_socket, 5);
	if (listen_status == -1) {
		printf("Listening failed\n");
		exit(1);
	}

	int client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1) {
		printf("Accepting client failed\n");
		exit(1);
	}

	char response[256];
	int recv_status = recv(client_socket, response, sizeof(response), 0);
	if (recv_status == -1) {
		printf("Receiving message from client failed\n");
		exit(1);
	}
	response[recv_status] = '\0';

	int send_status = send(client_socket, response, recv_status, 0);
	if (send_status == -1) {
		printf("Sending message to client failed\n");
		exit(1);
	}

	close(client_socket);

	return 0;
}
