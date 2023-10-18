#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <poll.h>

int main() {
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error1\n");
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
        printf("Error2\n");
    }

    if (listen(sockfd, 10)) {
        printf("Error3\n");
    }
    int highestFd = 0;
    struct fd_set readfds;
    FD_ZERO(&readfds);

    struct pollfd fds[10000];
    for (int i = 0; i < 10000; i++) {
        fds[i].fd = -1;
    }
    int pollFd = 0;
    char recvline[1000]; 

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        int new_sock = accept(sockfd, (struct sockaddr *)&client_addr, &length);
        if (new_sock < 0)
            printf("Error4\n");
        printf("New client accepted\n");

        FD_SET(new_sock, &readfds);
        highestFd = new_sock;

        // fds[pollFd].fd = new_sock;
        // fds[pollFd].events = POLLIN | POLLOUT | POLLERR;
        // ++pollFd;

        // int pollRes = poll(fds, 10000, 1000);
        // if (pollRes == -1)
        //     printf("ErrorPoll\n");
        // else if (pollRes == 0)
        //     printf("PollTimeout\n");
        // else {
        //     printf("PollSuccessful\n");
        //     for (int i = 0; i < 10000; i++) {
        //         if ((fds[i].revents & POLLIN) != 0) {
        //             printf("POLLIN Event found for %i\n", fds[i].fd);
        //             if (recv(fds[i].fd, recvline, 1000 - 1, 0) == 0) {
        //                 printf("Client disconnected for %i\n", fds[i].fd);
        //             }
        //             else {
        //                 printf("Got message %s\n", recvline);
        //             }
        //         }
        //         if ((fds[i].revents & POLLERR) != 0) {
        //             printf("Client disconnected for %i\n", fds[i].fd);
        //         }
        //     }
        // }


        if (select(highestFd + 1, &readfds, NULL, NULL, NULL) < 0) {
            printf("Error5\n");
        }
        printf("Select did select\n");
        char recvline[100000];
        memset(recvline, 0, 100000);
        while (recv(new_sock, recvline, 100000 - 1, 0) > 0) {
            printf("%s", recvline);
        }
        printf("End of loop\n");
    }
}

