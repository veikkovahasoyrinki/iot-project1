import socket

SERVER_ADDR = '2600:1900:4150:a91b::' 
SERVER_PORT = 8683 

message = "Testing connection"

# Create an IPv6 UDP socket and bind the socket to the server address and port
client_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

#Send to server
client_socket.sendto(message.encode(), (SERVER_ADDR, SERVER_PORT))

print(f"Sent message to [{SERVER_ADDR}]:{SERVER_PORT}: {message}")

# Receive a response from the server
client_socket.settimeout(5)  # Set a timeout for receiving the response (5 seconds)
try:
    response, server_address = client_socket.recvfrom(1024) 
    print(f"Received response from [{server_address[0]}]:{server_address[1]}: {response.decode()}")
except socket.timeout:
    print("No response received within the timeout period")

# Close the socket
client_socket.close()