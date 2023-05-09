///////////////////////////////////////////////////////////////////////////
//This is a simple client which is intended to connect to dev_null_server
//once connected this client will push 4G of data read from /dev/zero
//
//       python dev_zero_client.py <server_ip> <port>
//
// Author: Michael Colombo
// Date: 5/7/2023 
//
///////////////////////////////////////////////////////////////////////////

#! /usr/bin/env python

import socket
import sys

HOST = sys.argv[1]
PORT = int (sys.argv[2])

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to a remote server
sock.connect((HOST, PORT))
print("Connected to {}:{}".format(HOST, PORT))

# Open /dev/zero for reading
with open('/dev/zero', 'rb') as zero_file:
    # Read 4GB of data from /dev/zero
    bytes_to_read = 4 * 1024 * 1024 * 1024
    while bytes_to_read > 0:
        # Read data from /dev/zero in chunks of 1MB
        chunk_size = min(bytes_to_read, 1048576)
        data = zero_file.read(chunk_size)

        # Write the data to the socket
        sock.sendall(data)

        # Decrement the bytes left to read
        bytes_to_read -= chunk_size

# Close the socket
print("Closing connection to {}:{}".format(HOST, PORT))
sock.close()

