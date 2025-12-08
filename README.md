# Networking Projects

## Overview

This repository contains implementations of networking protocols and personal projects, designed to illustrate core concepts in network programming and systems.

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
└── README.md              # This file
```

## Technologies

- **Python**: High-level socket programming using the `socket` module
- **C**: Low-level socket programming using POSIX socket API
- **Protocols**: TCP (Transmission Control Protocol), UDP (User Datagram Protocol)

## Projects

### Python Implementation (`client-server/`)

Python-based client-server applications demonstrating both connection-oriented (TCP) and connectionless (UDP) communication patterns. These implementations showcase Python's high-level socket abstractions.

**Features:**
- UDP echo server with uppercase transformation (Port 12000)
- TCP echo server with uppercase transformation (Port 12001)
- Interactive clients with user input handling

See [client-server/README.md](client-server/README.md) for detailed documentation.

### C Implementation (`client-server-C/`)

Low-level C implementations using the POSIX socket API, providing insight into system-level network programming and resource management.

**Features:**
- TCP connection establishment and data exchange (Port 12000)
- Manual memory management and error handling
- Direct system call interface

See [client-server-C/README.md](client-server-C/README.md) for detailed documentation.

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

### C

```bash
# Compile the C programs
gcc client-server-C/TCP-server.c -o tcp-server
gcc client-server-C/TCP-client.c -o tcp-client

# Run server
./tcp-server

# In another terminal, run client
./tcp-client
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

3. **Client-Server Architecture**
   - Server lifecycle (bind, listen, accept)
   - Client connection patterns
   - Data serialization and transmission

4. **Language-Specific Approaches**
   - Python's high-level abstractions vs C's system-level control
   - Memory management across languages
   - Error handling patterns

## Network Concepts Demonstrated

- **Socket Types**: `SOCK_STREAM` (TCP), `SOCK_DGRAM` (UDP)
- **Address Families**: IPv4 (`AF_INET`)
- **Byte Order Conversion**: Network byte order vs host byte order
- **Blocking I/O**: Synchronous communication patterns
- **Port Management**: Binding to specific ports

## Testing

To verify the implementations work correctly:

1. Start a server in one terminal
2. Run the corresponding client in another terminal
3. Enter text in the client (for Python implementations)
4. Observe the server's response

Expected behavior:
- Python servers: Convert input to uppercase and echo back
- C server: Send a predefined welcome message

## Contributing

This is an educational project. When contributing:

1. Maintain code clarity and documentation
2. Follow existing naming conventions
3. Add error handling for new features
4. Update relevant README files
5. Test with both client and server components

## License

This project is provided as-is for educational purposes.

