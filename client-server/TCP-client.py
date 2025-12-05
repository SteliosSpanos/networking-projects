from socket import *

server_name = '127.0.0.1'
server_port = 12001

client_socket = socket(AF_INET, SOCK_STREAM)
client_socket.connect((server_name, server_port))

message = input("Input lowercase sentence: ")
if not str(message):
    print("Input must be a string")
    exit -1

client_socket.send(message.encode())

modified_message = client_socket.recv(1024)

print("From Server: ", modified_message.decode())

client_socket.close()
