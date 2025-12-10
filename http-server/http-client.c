#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		perror("socket");
		exit(1);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8080);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	int connect_status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	if (connect_status == -1) {
		perror("connect");
		close(client_socket);
		exit(1);
	}

	const char *request =
		"GET / HTTP/1.1\r\n"
		"Host: localhost\r\n"
		"Connection: close\r\n"
		"\r\n";

	int send_status = send(client_socket, request, strlen(request), 0);
	if (send_status == -1) {
		perror("send");
		close(client_socket);
		exit(1);
	}

	char buffer[4096];
	ssize_t bytes;

	printf("Response:\n");
	while ((bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
		buffer[bytes] = '\0';
		printf("%s", buffer);
	}

	close(client_socket);
	return 0;
}
