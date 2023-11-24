import socket

# Configure the server's IPv6 address and port
SERVER_IPV6_ADDRESS = '2600:1900:4150:a91b::'  # Replace with your server's IPv6 address
SERVER_PORT = 8683  # Replace with your server's UDP port

# Message to send to the server
message = "Hello from UDP client!"

# Create an IPv6 UDP socket
client_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

# Send the message to the server
client_socket.sendto(message.encode(), (SERVER_IPV6_ADDRESS, SERVER_PORT))

print(f"Sent message to [{SERVER_IPV6_ADDRESS}]:{SERVER_PORT}: {message}")

# Receive a response from the server
client_socket.settimeout(5)  # Set a timeout for receiving the response (5 seconds)
try:
    response, server_address = client_socket.recvfrom(1024)  # Buffer size
    print(f"Received response from [{server_address[0]}]:{server_address[1]}: {response.decode()}")
except socket.timeout:
    print("No response received within the timeout period")

# Close the socket
client_socket.close()