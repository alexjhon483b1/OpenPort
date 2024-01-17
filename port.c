#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

int isPortOpen(const char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd == -1) {
        perror("socket");
        return -1;  // Error creating socket
    }

    // Set the socket to non-blocking mode
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &(server_addr.sin_addr));

    int result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if (result == 0) {
        // Connection succeeded immediately
        close(sockfd);
        return 1;  // Port is open
    } else if (result < 0 && errno == EINPROGRESS) {
        // Connection in progress, wait for it to complete
        fd_set write_fds;
        FD_ZERO(&write_fds);
        FD_SET(sockfd, &write_fds);

        struct timeval timeout;
        timeout.tv_sec = 2;  // 2-second timeout
        timeout.tv_usec = 0;

        result = select(sockfd + 1, NULL, &write_fds, NULL, &timeout);

        if (result > 0 && FD_ISSET(sockfd, &write_fds)) {
            // Connection succeeded
            close(sockfd);
            return 1;  // Port is open
        } else {
            // Connection timed out or failed
            close(sockfd);
            return 0;  // Port is closed
        }
    } else {
        // Connection failed immediately
        close(sockfd);
        return 0;  // Port is closed
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP address> <Port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int result = isPortOpen(ip, port);

    if (result == 1) {
        printf("Port %d on %s is open.\n", port, ip);
    } else if (result == 0) {
        printf("Port %d on %s is closed.\n", port, ip);
    } else {
        fprintf(stderr, "Error checking port status.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

