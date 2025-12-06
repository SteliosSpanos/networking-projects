#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {

	int net_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(12000);
	server_address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
						     
	int status = connect(net_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	if (status == -1) {
		printf("COnnection failed\n");
		exit(1);
	}

	char response[256];
	recv(net_socket, &response, sizeof(response), 0);

	printf("From Server: %s\n", response);

	close(net_socket);

	return 0;
}
