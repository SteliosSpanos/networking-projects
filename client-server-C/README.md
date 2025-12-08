# C Client-Server Implementation

Low-level TCP client-server implementation in C using the POSIX socket API. This implementation demonstrates system-level network programming with direct system calls and manual resource management.

## Overview

This directory contains C implementations of a basic TCP client-server application using the BSD/POSIX socket interface. These programs illustrate low-level networking concepts including socket creation, connection management, and data transmission without high-level abstractions.

## Files

- **TCP-server.c**: TCP server that accepts a connection and echoes received messages
- **TCP-client.c**: TCP client that connects to the server and exchanges messages

## Network Configuration

| Component | Port  | Protocol | Address   |
|-----------|-------|----------|-----------|
| Server    | 12000 | TCP      | 0.0.0.0   |
| Client    | 12000 | TCP      | 127.0.0.1 |

## Prerequisites

### Compiler
- GCC 4.8 or higher (or any C99-compatible compiler)
- Alternative: Clang, ICC, or other POSIX-compliant C compilers

### System Requirements
- POSIX-compliant operating system (Linux, macOS, BSD, Unix)
- Standard C library with socket support
- Headers: `sys/socket.h`, `netinet/in.h`, `arpa/inet.h`

## Installation

### Compilation

**Compile both programs:**
```bash
gcc TCP-server.c -o tcp-server
gcc TCP-client.c -o tcp-client
```

**With debugging symbols:**
```bash
gcc -g TCP-server.c -o tcp-server
gcc -g TCP-client.c -o tcp-client
```

**With warnings enabled (recommended):**
```bash
gcc -Wall -Wextra -pedantic TCP-server.c -o tcp-server
gcc -Wall -Wextra -pedantic TCP-client.c -o tcp-client
```

**With optimization:**
```bash
gcc -O2 TCP-server.c -o tcp-server
gcc -O2 TCP-client.c -o tcp-client
```

### Makefile (Optional)

Create a `Makefile` for easier compilation:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
TARGETS = tcp-server tcp-client

all: $(TARGETS)

tcp-server: TCP-server.c
	$(CC) $(CFLAGS) $< -o $@

tcp-client: TCP-client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)

.PHONY: all clean
```

Then compile with:
```bash
make
```

## Usage

### Running the Server

**Start the server:**
```bash
./tcp-server
```

The server will:
1. Create a TCP socket
2. Bind to port 12000 on all interfaces (0.0.0.0)
3. Listen for incoming connections
4. Accept one client connection
5. Receive a message from the client
6. Echo the message back
7. Close the connection and exit

**Note:** Current implementation handles only one client and then exits.

### Running the Client

**In a separate terminal, start the client:**
```bash
./tcp-client
```

**Example interaction:**
```
Enter a message: Hello Server
From Server: Hello Server
```

The client will:
1. Create a TCP socket
2. Connect to 127.0.0.1:12000
3. Prompt for user input
4. Send the message to the server
5. Receive and display the server's response
6. Close the connection and exit

## Implementation Details

### TCP Server (TCP-server.c)

**System calls used:**
```c
socket()    // Create socket endpoint
bind()      // Bind socket to address
listen()    // Mark socket as passive
accept()    // Accept incoming connection
recv()      // Receive data
send()      // Send data
close()     // Close socket
```

**Data structures:**
```c
struct sockaddr_in {
    sa_family_t    sin_family;  // AF_INET (IPv4)
    in_port_t      sin_port;    // Port number (network byte order)
    struct in_addr sin_addr;    // IPv4 address
    char           sin_zero[8]; // Padding
};
```

**Server flow:**
1. **Socket Creation** (line 11): `socket(AF_INET, SOCK_STREAM, 0)`
   - AF_INET: IPv4 address family
   - SOCK_STREAM: TCP socket type
   - 0: Default protocol (TCP)

2. **Address Configuration** (lines 17-20):
   - sin_family: AF_INET (IPv4)
   - sin_port: htons(12000) - Convert port to network byte order
   - sin_addr.s_addr: INADDR_ANY - Bind to all interfaces

3. **Binding** (line 22): `bind(server_socket, &server_address, sizeof(server_address))`
   - Associates socket with local address and port

4. **Listening** (line 28): `listen(server_socket, 5)`
   - Marks socket as passive (accepting connections)
   - Backlog of 5 pending connections

5. **Accepting** (line 34): `accept(server_socket, NULL, NULL)`
   - Blocks until client connects
   - Returns new socket for this specific connection
   - Original socket remains for accepting more connections

6. **Receiving** (line 41): `recv(client_socket, receive, sizeof(receive), 0)`
   - Reads data from client
   - Returns number of bytes received

7. **Null-termination** (line 42): `receive[recv_status] = '\0'`
   - Adds string terminator for printf

8. **Sending** (line 48): `send(client_socket, receive, recv_status, 0)`
   - Echoes received data back to client

9. **Cleanup** (line 54): `close(client_socket)`
   - Closes client connection

### TCP Client (TCP-client.c)

**System calls used:**
```c
socket()    // Create socket endpoint
connect()   // Establish connection to server
send()      // Send data
recv()      // Receive data
close()     // Close socket
```

**Client flow:**
1. **Socket Creation** (line 12): `socket(AF_INET, SOCK_STREAM, 0)`

2. **Server Address Configuration** (lines 18-21):
   - sin_family: AF_INET
   - sin_port: htons(12000)
   - sin_addr.s_addr: inet_addr("127.0.0.1") - Server IP

3. **Connection** (line 23): `connect(client_socket, &server_address, sizeof(server_address))`
   - Initiates TCP three-way handshake
   - Blocks until connection established or fails

4. **User Input** (lines 29-31):
   - Prompts for message
   - **Note:** scanf format should be `"%255s"` not `"255%s"`

5. **Sending** (line 32): `send(client_socket, message, strlen(message), 0)`
   - Transmits message to server

6. **Receiving** (line 39): `recv(client_socket, response, sizeof(response), 0)`
   - Waits for server response

7. **Display and Cleanup** (lines 46-48):
   - Prints server response
   - Closes socket

## Socket Programming Concepts

### Address Families
- **AF_INET**: IPv4 Internet protocols
- **AF_INET6**: IPv6 Internet protocols (not used here)

### Socket Types
- **SOCK_STREAM**: Connection-oriented, reliable, ordered byte stream (TCP)
- **SOCK_DGRAM**: Connectionless, unreliable datagrams (UDP, not used here)

### Byte Order Conversion
- **htons()**: Host TO Network Short - Convert 16-bit port to big-endian
- **htonl()**: Host TO Network Long - Convert 32-bit IP to big-endian
- **ntohs()**: Network TO Host Short
- **ntohl()**: Network TO Host Long

Network protocols use big-endian byte order regardless of host architecture.

### Blocking Behavior
All socket operations in these programs are blocking:
- `accept()`: Blocks until client connects
- `connect()`: Blocks until connection established
- `recv()`: Blocks until data arrives
- `send()`: Blocks until data buffered for transmission

## Error Handling

Current implementations include basic error checking:

```c
if (socket_fd == -1) {
    printf("Socket failed\n");
    exit(1);
}
```

## Performance Considerations

### Buffer Sizes
- Current: 256 bytes
- Trade-off: Larger buffers use more memory but reduce system calls
- Typical values: 4KB-64KB for production

### Connection Overhead
- TCP three-way handshake adds latency
- Keep-alive connections reduce overhead for multiple messages
- Consider connection pooling for high-frequency communication

### System Call Overhead
- Each send/recv is a system call (context switch)
- Buffering multiple messages reduces overhead
- Use sendmsg/recvmsg for scatter-gather I/O

## License

Educational implementation for learning purposes.
