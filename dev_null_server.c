// This is a server that will open a socket on port 8080. It will read from socket and write to /dev/null.\\
// This application can be used for testing network throughput or possibly other learning exercises
//
// My end goal with this server is to add the option for random delays to show what happens when an application
// does not read from it's socket buffer fast enough

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    int server_fd = 0
    int new_socket = 0
    int valread = 0
        
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    // Set address info
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    listen(server_fd, 3);
    
    // Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
        // Read from socket and write to /dev/null
    while ((valread = read(new_socket, buffer, 1024)) > 0) {
        write(open("/dev/null", O_WRONLY), buffer, valread);
        memset(buffer, 0, 1024);
    }
    
    // Close sockets and exit
    close(new_socket);
    close(server_fd);
    return 0;
}
