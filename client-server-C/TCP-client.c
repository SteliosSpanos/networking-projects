#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		perror("socket:");
		exit(1);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(12000);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
						     
	int status = connect(client_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	if (status == -1) {
		perror("connect:");
		exit(1);
	}

	char message[256];
	printf("Enter a message: ");
	fgets(message, sizeof(message), stdin);
	int send_status = send(client_socket, message, strlen(message), 0);
	if (send_status == -1) {
		perror("send:");
		exit(1);
	}

	char response[256];
	int recv_status = recv(client_socket, response, sizeof(response), 0);
	if (recv_status == -1) {
		perror("recv:");
		exit(1);
	}
	response[recv_status] = '\0';

	printf("From Server: %s\n", response);

	close(client_socket);

	return 0;
}
