# HTTP Server Implementation in C

A lightweight HTTP/1.1 web server implemented in C using POSIX sockets. This server demonstrates application-layer protocol implementation on top of TCP, featuring static file serving, MIME type detection, and proper HTTP error handling.

## Overview

This HTTP server builds upon fundamental TCP socket programming to implement the HTTP/1.1 protocol. It serves static files from a document root directory, handles multiple request types, and provides proper error responses with HTTP status codes.

## Files

- **http-server.c**: HTTP/1.1 server implementation serving static files
- **http-client.c**: Simple HTTP client for testing the server
- **www/**: Document root directory containing static files to serve

## Network Configuration

| Component | Port | Protocol | Address   |
|-----------|------|----------|-----------|
| Server    | 8080 | HTTP/1.1 | 0.0.0.0   |
| Client    | 8080 | HTTP/1.1 | 127.0.0.1 |

## Features

### Core Functionality
- HTTP/1.1 protocol implementation
- Static file serving from document root
- GET and HEAD method support
- Automatic MIME type detection
- Proper HTTP status codes

### Security
- Directory traversal protection (blocks `..` in paths)
- Path validation before file access
- Input validation for HTTP request format

### Error Handling
- 200 OK - Successful file retrieval
- 400 Bad Request - Malformed HTTP request
- 403 Forbidden - Security policy violation
- 404 Not Found - Requested file doesn't exist
- 405 Method Not Allowed - Unsupported HTTP method
- 500 Internal Server Error - Server-side failures

### MIME Types Supported
- HTML: `text/html`
- CSS: `text/css`
- JavaScript: `application/javascript`
- JSON: `application/json`
- JPEG: `image/jpeg`
- PNG: `image/png`
- GIF: `image/gif`
- Plain text: `text/plain`
- Default: `application/octet-stream`

## Prerequisites

### Compiler
- GCC 4.8 or higher
- Any C99-compatible compiler (Clang, ICC, etc.)

### System Requirements
- POSIX-compliant operating system (Linux, macOS, BSD, Unix)
- Standard C library with socket support
- File system access for serving static files

### Required Headers
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
```

## Installation

### Compilation

**Compile the server:**
```bash
gcc http-server.c -o http-server
```

**Compile the test client:**
```bash
gcc http-client.c -o http-client
```

**With compiler warnings (recommended):**
```bash
gcc -Wall -Wextra -pedantic -std=c99 http-server.c -o http-server
gcc -Wall -Wextra -pedantic -std=c99 http-client.c -o http-client
```

**With debugging symbols:**
```bash
gcc -g http-server.c -o http-server
```

### Setup Document Root

Create the directory for serving files:
```bash
mkdir www
```

Create test files:
```bash
echo '<html><head><title>Test Page</title></head><body><h1>Hello from HTTP Server!</h1></body></html>' > www/index.html

echo '<html><body><h1>About</h1><p>This is the about page.</p></body></html>' > www/about.html

echo 'body { color: blue; font-family: Arial; }' > www/style.css
```

## Usage

### Running the Server

**Start the server:**
```bash
./http-server
```

**Expected output:**
```
HTTP server listening on port 8080
```

The server will:
1. Bind to port 8080 on all interfaces
2. Listen for incoming HTTP connections
3. Accept connections indefinitely
4. Parse HTTP requests
5. Serve files from `www/` directory
6. Log each request to stdout
7. Handle errors with appropriate HTTP responses

### Testing the Server

**Method 1: Using curl (recommended for testing)**
```bash
# Test root path
curl http://localhost:8080/

# Test specific file
curl http://localhost:8080/about.html

# View full HTTP headers and response
curl -v http://localhost:8080/

# Test 404 error
curl http://localhost:8080/nonexistent.html

# Test directory traversal protection (should return 403)
curl http://localhost:8080/../etc/passwd

# Test HEAD request
curl -I http://localhost:8080/

# Test CSS file (check Content-Type)
curl -v http://localhost:8080/style.css
```

**Method 2: Using web browser**
```
http://localhost:8080/
http://localhost:8080/about.html
```

Open Developer Tools (F12) to view HTTP headers and responses.

**Method 3: Using the test client**
```bash
./http-client
```

**Method 4: Using telnet (manual HTTP requests)**
```bash
telnet localhost 8080
```

Then type:
```
GET /index.html HTTP/1.1
Host: localhost

```
*Press Enter twice after the Host header*

## Implementation Details

### Server Architecture

**Main Loop:**
```c
while(1) {
    accept connection
    handle_request()
    close connection
}
```

**Request Handling Flow:**
1. Receive HTTP request via `recv()`
2. Parse request line (method, path, version)
3. Validate HTTP method (GET or HEAD)
4. Check for directory traversal attempts
5. Map URL path to filesystem path
6. Open and read file
7. Determine MIME type from file extension
8. Send HTTP response headers
9. Send file content (for GET, skip for HEAD)
10. Clean up resources

### HTTP Request Format

The server expects standard HTTP/1.1 requests:
```
GET /path HTTP/1.1\r\n
Host: localhost\r\n
[Additional headers]\r\n
\r\n
```

**Parsed components:**
- **Method**: GET, HEAD
- **Path**: URL path (e.g., `/`, `/about.html`)
- **Version**: HTTP/1.1

### HTTP Response Format

**Success Response (200 OK):**
```
HTTP/1.1 200 OK\r\n
Content-Type: text/html\r\n
Content-Length: 123\r\n
Connection: close\r\n
\r\n
[file content]
```

**Error Response (404 Not Found):**
```
HTTP/1.1 404 Not Found\r\n
Content-Type: text/html\r\n
Content-Length: 98\r\n
Connection: close\r\n
\r\n
<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>
```

### Key Functions

**`handle_request(int client_socket)`**
- Receives and parses HTTP request
- Validates method and path
- Opens and reads requested file
- Sends HTTP response with appropriate headers
- Handles all error conditions

**`send_error(int client_socket, int status_code, const char *status_message)`**
- Constructs HTML error page
- Formats HTTP error response
- Sends to client

**`get_mime_type(const char *filepath)`**
- Extracts file extension
- Maps extension to MIME type
- Returns appropriate Content-Type header value

### Security Features

**Directory Traversal Protection:**
```c
if (strstr(path, "..") != NULL) {
    send_error(client_socket, 403, "Forbidden");
    return;
}
```

Blocks requests containing `..` to prevent access to files outside the document root.

**Path Mapping:**
- Root path `/` maps to `www/index.html`
- Other paths prepend `www/` (e.g., `/about.html` → `www/about.html`)

### Socket Options

**SO_REUSEADDR:**
```c
setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

Allows immediate server restart without waiting for TIME_WAIT state to expire.

## Differences from TCP Echo Server

| Aspect | TCP Echo Server | HTTP Server |
|--------|----------------|-------------|
| **Protocol Layer** | Transport (TCP) | Application (HTTP/1.1) |
| **Data Format** | Raw bytes | Structured HTTP messages |
| **Request Parsing** | None | Parse method, path, headers |
| **Response Format** | Echo input | HTTP status + headers + body |
| **File Operations** | None | Read files from disk |
| **MIME Types** | N/A | Content-Type detection |
| **Error Handling** | Basic | HTTP status codes |
| **Security** | None | Path validation |
| **Complexity** | ~60 lines | ~210 lines |

## Error Handling

### System Call Errors

All system calls check for errors:
```c
if (socket_fd == -1) {
    perror("socket");
    exit(1);
}
```

**Error handling strategy:**
- Fatal errors (socket creation, bind, listen): Exit with error message
- Client-specific errors (recv, send, file operations): Return from handler, continue serving other clients
- All errors use `perror()` for diagnostic output

### Common Errors

**Address Already in Use:**
```
bind: Address already in use
```
**Solution:** Wait 30-60 seconds, or use `SO_REUSEADDR` (already implemented)

**Permission Denied:**
```
bind: Permission denied
```
**Solution:** Use port > 1024, or run with appropriate permissions

**File Not Found:**
```
Method: GET, Path: /missing.html, Version: HTTP/1.1
```
Server sends 404 response to client.

## Testing

### Test Cases

**Successful requests:**
```bash
curl http://localhost:8080/                    # Should return index.html
curl http://localhost:8080/index.html          # Should return index.html
curl http://localhost:8080/about.html          # Should return about.html
curl -I http://localhost:8080/                 # HEAD request, headers only
```

**Error conditions:**
```bash
curl http://localhost:8080/nonexistent.html    # 404 Not Found
curl http://localhost:8080/../secret.txt       # 403 Forbidden
curl -X POST http://localhost:8080/            # 405 Method Not Allowed
curl http://localhost:8080/                    # With malformed request: 400
```

**MIME types:**
```bash
curl -v http://localhost:8080/style.css        # Should show Content-Type: text/css
curl -v http://localhost:8080/index.html       # Should show Content-Type: text/html
```

### Expected Server Output

```
HTTP server listening on port 8080
Method: GET, Path: /, Version: HTTP/1.1
Method: GET, Path: /about.html, Version: HTTP/1.1
Method: GET, Path: /nonexistent.html, Version: HTTP/1.1
Method: GET, Path: /../etc/passwd, Version: HTTP/1.1
```

## Troubleshooting

### Server Won't Start

**Problem:** Port already in use
**Check:** `netstat -tuln | grep 8080` or `ss -tuln | grep 8080`
**Solution:** Kill existing process or change port

**Problem:** Permission denied
**Solution:** Use port above 1024 or run with appropriate permissions

### Client Connection Refused

**Problem:** Server not running
**Check:** `ps aux | grep http-server`
**Solution:** Start the server first

**Problem:** Firewall blocking
**Check:** `sudo iptables -L`
**Solution:** Allow port 8080

### Server Crashes

**Problem:** Segmentation fault
**Debug:** Compile with `-g` and run in `gdb`
**Common causes:** Buffer overflow, null pointer, use-after-free

### Files Not Found

**Problem:** 404 errors for existing files
**Check:** Ensure files are in `www/` directory
**Check:** Verify file permissions: `ls -la www`

## HTTP/1.1 Compliance

### Implemented Features
- Request parsing (method, path, version)
- Response status line
- Content-Type and Content-Length headers
- Connection: close
- GET and HEAD methods
- Proper CRLF line endings

### Not Implemented
- Persistent connections (keep-alive)
- Chunked transfer encoding
- Content negotiation
- Conditional requests (If-Modified-Since, etc.)
- POST, PUT, DELETE methods
- Query string parsing
- Host-based virtual hosting
- Compression (gzip, deflate)

## License

Educational implementation for learning purposes.

