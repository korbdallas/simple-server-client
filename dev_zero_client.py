import socket
import sys

HOST = sys.argv[1]
PORT = int (sys.argv[2])

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to a remote server
#server_address = (sys.argv[1], sys.argv[2])
sock.connect((HOST, PORT))
print(" Connected to " + HOST + ":" + str(PORT))

# Open /dev/zero for reading
with open('/dev/zero', 'rb') as zero_file:
    # Read 4GB of data from /dev/zero
    bytes_to_read = 4 * 1024 * 1024 * 1024
    while bytes_to_read > 0:
        # Read data from /dev/zero in chunks of 1024 bytes
        chunk_size = min(bytes_to_read, 1024)
        data = zero_file.read(chunk_size)

        # Write the data to the socket
        sock.sendall(data)

        # Decrement the bytes left to read
        bytes_to_read -= chunk_size

# Close the socket
sock.close()

