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

void send_error(int client_socket, int status_code, const char *message);

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

	char filepath[512];
	if (strstr(path, "..") != NULL) {
		send_error(client_socket, 403, "Forbidden");
		return;
	}

	if (strcmp(path, "/") == 0) {
		snprintf(filepath, sizeof(filepath), "www/index.html");
	}
	else {
		snprintf(filepath, sizeof(filepath), "www%s", path);
	}


	int fd;
	struct stat file_stat;
	char *file_content;

	fd = open(filepath, O_RDONLY);
	if (fd == -1) {
		send_error(client_socket, 404, "Not Found");
		return;
	}

	if (fstat(fd, &file_stat) == -1) {
		close(fd);
		send_error(client_socket, 500, "Internal Server Error");
		return;
	}

	file_content = malloc(file_stat.st_size);
	if (read(fd, file_content, file_stat.st_size) != file_stat.st_size) {
		close(fd);
		free(file_content);
		send_error(client_socket, 500, "Internal Server Error");
		return;
	}
	close(fd);

	char response_header[1024];
	int header_len = snprintf(response_header, sizeof(response_header),
		"HTTP/1.1 200 OK\r\n"
      		"Content-Type: text/html\r\n"
      		"Content-Length: %ld\r\n"
      		"Connection: close\r\n"
      		"\r\n",
      		file_stat.st_size
	);

	int send_header = send(client_socket, response_header, header_len, 0);
	if (send_header == -1) {
		perror("send:");
		exit(1);
	}

	int send_body = send(client_socket, file_content, file_stat.st_size, 0);
	if (send_body == -1) {
		perror("send:");
		exit(1);
	}

	free(file_content);

}

void send_error(int client_socket, int status_code, const char *message) {
	char body[512];
      	int body_len = snprintf(body, sizeof(body),
        	"<html><head><title>%d %s</title></head>"
          	"<body><h1>%d %s</h1></body></html>",
          	status_code, status_message, status_code, status_message
	);

  	char response[1024];
      	int response_len = snprintf(response, sizeof(response),
          	"HTTP/1.1 %d %s\r\n"
          	"Content-Type: text/html\r\n"
          	"Content-Length: %d\r\n"
          	"Connection: close\r\n"
          	"\r\n"
          	"%s",
          	status_code, status_message, body_len, body
	);

      	send(client_socket, response, response_len, 0);
}
