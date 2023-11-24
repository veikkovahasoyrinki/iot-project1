import socket
import os
import sys
import time
import datetime



SERVER_ADDR = '2600:1900:4150:a91b::'  
SERVER_PORT = 8683  

INPUT_DATA = 'data.csv'

# Create an IPv6 UDP socket and bind the socket to the server address and port
server_socket = socket.socket(socket.AF_INET6, socket.SOCK_DGRAM)

server_socket.bind((SERVER_ADDR, SERVER_PORT))

print("Hi from UDP server\n")
ACK_RESPONSE = "Test ok"
responseBytes = str.encode(ACK_RESPONSE)
#Listening
print("Checking if " + INPUT_DATA + " exists in the current folder\n")
filePath = './' + INPUT_DATA
if not os.path.isfile(filePath):
    print("Creating the data file\n")
    open(INPUT_DATA, 'x')

print(f"UDP server listening on [{SERVER_ADDR}]:{SERVER_PORT}")
while True:
    
    message, address = server_socket.recvfrom(1024)  
    sttime = datetime.datetime.now().strftime('%Y%m%d_%H:%M:%S - ')
    try:
        message = message.decode('utf-8') 
    except:
        print("Corrupted UDP packet recieved, skipping...\n")
    else:
        print(f"Received message from [{address[0]}]:{address[1]}: {message}: \n")

        if "Testing" not in message:
            with open(INPUT_DATA, 'a') as file:
                file.write(sttime + message + '\n')
            # file.write('-' * 20 + '\n')
        else:
            #Send a response
            print("Sending response to connection test: Test ok")
            server_socket.sendto(responseBytes, address)


server_socket.close()