# Networking Projects

## Overview

This repository contains implementations of networking protocols and personal projects, designed to illustrate core concepts in network programming and systems. The projects range from basic TCP/UDP echo servers to a full HTTP/1.1 web server implementation.

### Project Structure

```
networking-projects/
├── client-server/          # Python implementations
│   ├── UDP-client.py
│   ├── UDP-server.py
│   ├── TCP-client.py
│   └── TCP-server.py
├── client-server-C/        # C implementations
│   ├── TCP-client.c
│   └── TCP-server.c
├── http-server/            # HTTP/1.1 server in C
│   ├── http-server.c
│   ├── http-client.c
│   └── www/
└── README.md              # This file
```

## Technologies

- **Python**: High-level socket programming using the `socket` module
- **C**: Low-level socket programming using POSIX socket API
- **Protocols**: TCP, UDP (Transport Layer), HTTP/1.1 (Application Layer)

## Projects

### Python Implementation (`client-server/`)

Python-based client-server applications demonstrating both connection-oriented (TCP) and connectionless (UDP) communication patterns. These implementations showcase Python's high-level socket abstractions.

**Features:**
- UDP echo server with uppercase transformation (Port 12000)
- TCP echo server with uppercase transformation (Port 12001)
- Interactive clients with user input handling

See [client-server/README.md](client-server/README.md) for detailed documentation.

### C TCP Implementation (`client-server-C/`)

Low-level C implementations using the POSIX socket API, providing insight into system-level network programming and resource management.

**Features:**
- TCP connection establishment and data exchange (Port 12000)
- Manual memory management and error handling
- Direct system call interface

See [client-server-C/README.md](client-server-C/README.md) for detailed documentation.

### HTTP Server (`http-server/`)

Full-featured HTTP/1.1 web server implementation in C, demonstrating application-layer protocol development on top of TCP sockets.

**Features:**
- HTTP/1.1 protocol implementation (Port 8080)
- Static file serving with automatic MIME type detection
- GET and HEAD method support
- Directory traversal protection
- Proper HTTP status codes (200, 400, 403, 404, 405, 500)
- Support for HTML, CSS, JavaScript, images, and more

See [http-server/README.md](http-server/README.md) for detailed documentation.

## Prerequisites

### Python Projects
- Python 3.6 or higher
- Standard library (no external dependencies)

### C Projects
- GCC compiler (or any C99-compatible compiler)
- POSIX-compliant system (Linux, macOS, Unix)
- Standard C library with socket support

## Quick Start

### Python

```bash
# Start UDP server
python client-server/UDP-server.py

# In another terminal, run UDP client
python client-server/UDP-client.py
```

### C TCP Echo Server

```bash
# Compile the C programs
gcc client-server-C/TCP-server.c -o tcp-server
gcc client-server-C/TCP-client.c -o tcp-client

# Run server
./tcp-server

# In another terminal, run client
./tcp-client
```

### HTTP Server

```bash
# Compile
cd http-server
gcc http-server.c -o http-server

# Create document root
mkdir -p www
echo '<html><body><h1>Hello World</h1></body></html>' > www/index.html

# Run server
./http-server

# Test with curl or browser
curl http://localhost:8080/
# Or open http://localhost:8080/ in browser
```

## Learning Objectives

This repository helps developers understand:

1. **Socket Programming Fundamentals**
   - Creating and configuring sockets
   - Binding to network interfaces
   - Establishing connections

2. **Protocol Differences**
   - TCP: Connection-oriented, reliable, ordered delivery
   - UDP: Connectionless, unreliable, simple message passing
   - HTTP: Application-layer protocol built on TCP

3. **Client-Server Architecture**
   - Server lifecycle (bind, listen, accept)
   - Client connection patterns
   - Data serialization and transmission

4. **Language-Specific Approaches**
   - Python's high-level abstractions vs C's system-level control
   - Memory management across languages
   - Error handling patterns

5. **Protocol Layers**
   - Transport Layer: TCP/UDP socket operations
   - Application Layer: HTTP protocol implementation
   - How higher-layer protocols build on lower layers

## Network Concepts Demonstrated

- **Socket Types**: `SOCK_STREAM` (TCP), `SOCK_DGRAM` (UDP)
- **Address Families**: IPv4 (`AF_INET`)
- **Byte Order Conversion**: Network byte order vs host byte order
- **Blocking I/O**: Synchronous communication patterns
- **Port Management**: Binding to specific ports
- **HTTP Protocol**: Request parsing, response formatting, status codes
- **MIME Types**: Content type detection and headers
- **Security**: Input validation, directory traversal protection

## Testing

To verify the implementations work correctly:

### TCP/UDP Servers
1. Start a server in one terminal
2. Run the corresponding client in another terminal
3. Enter text in the client (for Python implementations)
4. Observe the server's response

Expected behavior:
- Python servers: Convert input to uppercase and echo back
- C TCP server: Echo back the received message

### HTTP Server
```bash
# Start server
./http-server

# Test with curl
curl http://localhost:8080/
curl http://localhost:8080/about.html
curl http://localhost:8080/nonexistent.html  # 404 error

# Or use a web browser
# Open http://localhost:8080/
```

## Architecture Notes

### Protocol Layering
The projects demonstrate different protocol layers:
- **Transport Layer**: TCP/UDP socket operations (client-server, client-server-C)
- **Application Layer**: HTTP protocol on top of TCP (http-server)

### From TCP to HTTP
The HTTP server builds directly on TCP concepts:
- Same socket creation, bind, listen, accept operations
- Adds HTTP request parsing and response formatting
- Adds file I/O for serving static content
- Implements application-level error handling with HTTP status codes

## License

This project is provided as-is for educational purposes.
