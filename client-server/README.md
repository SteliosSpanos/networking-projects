# Python Client-Server Implementation

Python-based implementations of UDP and TCP client-server applications demonstrating fundamental network programming concepts using Python's `socket` module.

## Overview

This directory contains four Python programs that showcase both connectionless (UDP) and connection-oriented (TCP) communication patterns. All implementations use Python's standard library and require no external dependencies.

## Files

### UDP Implementation

- **UDP-server.py**: UDP echo server that converts messages to uppercase
- **UDP-client.py**: UDP client that sends messages and receives responses

### TCP Implementation

- **TCP-server.py**: TCP echo server that converts messages to uppercase
- **TCP-client.py**: TCP client that establishes a connection and exchanges messages

## Network Configuration

| Protocol | Port  | Address   | Socket Type   |
|----------|-------|-----------|---------------|
| UDP      | 12000 | 127.0.0.1 | SOCK_DGRAM    |
| TCP      | 12001 | 127.0.0.1 | SOCK_STREAM   |

## Prerequisites

- Python 3.6 or higher
- Operating system with socket support (Linux, macOS, Windows)

## Installation

No installation required. The programs use only Python's standard library.

## Usage

### UDP Echo Server

**Start the server:**
```bash
python UDP-server.py
```

**Expected output:**
```
The server is ready to receive
```

The server runs indefinitely, waiting for incoming UDP datagrams on port 12000.

**Run the client:**
```bash
python UDP-client.py
```

**Example interaction:**
```
Input lowercase sentence: hello world
HELLO WORLD
```

The client sends the message to the server, which converts it to uppercase and sends it back.

### TCP Echo Server

**Start the server:**
```bash
python TCP-server.py
```

**Expected output:**
```
The server is ready to receive
```

The server listens on port 12001 and accepts TCP connections.

**Run the client:**
```bash
python TCP-client.py
```

**Example interaction:**
```
Input lowercase sentence: networking is fun
From Server:  NETWORKING IS FUN
```

The client establishes a TCP connection, sends the message, receives the uppercase response, and closes the connection.

## Implementation Details

### UDP Server (UDP-server.py)

**Key operations:**
1. Creates a UDP socket with `SOCK_DGRAM`
2. Binds to all interfaces ('') on port 12000
3. Enters infinite loop to handle datagrams
4. Uses `recvfrom()` to receive data and client address
5. Converts message to uppercase
6. Uses `sendto()` to send response back to client

**Socket characteristics:**
- Connectionless: No persistent connection with clients
- Stateless: Each message is independent
- Best-effort delivery: No guarantee of delivery or order

### UDP Client (UDP-client.py)

**Key operations:**
1. Creates a UDP socket
2. Sends message to server IP and port using `sendto()`
3. Waits for response using `recvfrom()`
4. Displays response and closes socket

**Note:** The client validates input is a string (line 10-12), though the validation logic could be improved.

### TCP Server (TCP-server.py)

**Key operations:**
1. Creates a TCP socket with `SOCK_STREAM`
2. Binds to all interfaces on port 12001
3. Calls `listen(1)` to mark socket as passive and set backlog
4. Enters infinite loop accepting connections
5. Uses `accept()` to get new connection socket
6. Receives data using `recv()` with 1024-byte buffer
7. Converts to uppercase and sends back
8. Closes connection socket (not listening socket)

**Socket characteristics:**
- Connection-oriented: Three-way handshake before data transfer
- Reliable: Guaranteed delivery in order
- Stream-based: Data treated as continuous byte stream

### TCP Client (TCP-client.py)

**Key operations:**
1. Creates a TCP socket
2. Calls `connect()` to establish connection to server
3. Sends message using `send()`
4. Receives response using `recv()`
5. Displays response and closes socket

**Connection lifecycle:**
- TCP three-way handshake on `connect()`
- Data exchange over established connection
- FIN/ACK handshake on `close()`

## Protocol Comparison

### UDP Characteristics

**Advantages:**
- Lower overhead (no connection establishment)
- Faster for small messages
- Supports broadcast and multicast
- Suitable for real-time applications tolerating packet loss

**Disadvantages:**
- No delivery guarantee
- No order guarantee
- No congestion control
- Application must handle lost packets

### TCP Characteristics

**Advantages:**
- Reliable delivery
- Ordered delivery
- Flow control and congestion control
- Suitable for applications requiring data integrity

**Disadvantages:**
- Higher overhead (connection establishment)
- Head-of-line blocking
- Not suitable for real-time applications with latency constraints

## Code Examples

### Creating a UDP Socket
```python
from socket import *

# Create UDP socket
client_socket = socket(AF_INET, SOCK_DGRAM)

# Send data (no connection needed)
client_socket.sendto(message.encode(), (server_name, server_port))

# Receive data with source address
data, address = client_socket.recvfrom(buffer_size)
```

### Creating a TCP Socket
```python
from socket import *

# Create TCP socket
client_socket = socket(AF_INET, SOCK_STREAM)

# Establish connection
client_socket.connect((server_name, server_port))

# Send and receive data
client_socket.send(message.encode())
data = client_socket.recv(buffer_size)

# Close connection
client_socket.close()
```

## License

Educational implementation for learning purposes.
