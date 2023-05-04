///////////////////////////////////////////////////////////////////////////
//This is a simple client which is intended to connect to dev_null_server
//once connected this client will push 4G of data read from /dev/zero
//
//       Usage: ./dev_zero_client <server_ip> <port>
//
///////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <fcntl.h>
#include <stdint.h>

#define BUFFER_SIZE (1 << 20) // 1 MB buffer size

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    int zero_fd;
    int total_bytes_sent = 0;
    unsigned long bytes_to_send = 4u * 1014u * 1024u * 1024u; // 4 GB

    // Open /dev/zero for reading
    if ((zero_fd = open("/dev/zero", O_RDONLY)) < 0) {
        perror("Error opening /dev/zero");
        exit(1);
    }

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up the server address
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[2]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portno);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Read from /dev/zero and write to the socket
    while (total_bytes_sent < bytes_to_send) {
        int bytes_read = read(zero_fd, buffer, sizeof(buffer));
        if (bytes_read < 0) {
            perror("Error reading from /dev/zero");
            exit(1);
        }
        int bytes_sent = write(sockfd, buffer, bytes_read);
        if (bytes_sent < 0) {
            perror("Error writing to socket");
            exit(1);
        }
        total_bytes_sent += bytes_sent;
    }

    // Close the socket and /dev/zero file descriptor
    close(sockfd);
    close(zero_fd);

    return 0;
}

