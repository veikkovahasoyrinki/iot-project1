import socket

SERVER_ADDR = '2600:1900:4150:a91b::'  
SERVER_PORT = 8683  

INPUT_DATA = 'data.txt'

# Create an IPv6 UDP socket and bind the socket to the server address and port
server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

server_socket.bind((SERVER_ADDR, SERVER_PORT))

print(f"UDP server listening on [{SERVER_ADDR}]:{SERVER_PORT}")

#Listening
while True:
    message, address = server_socket.recvfrom(1024)  
    message = message.decode('utf-8') 
    print(f"Received message from [{address[0]}]:{adbress[1]}: {message}: \n")
    print(message)
    with open(INPUT_DATA, 'a') as file:
        file.write(message + '\n')
        file.write('-' * 20 + '\n')


server_socket.close()