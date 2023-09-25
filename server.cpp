#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server_socket, client_socket;
    sockaddr_in server_address, client_address;
    socklen_t client_length;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(server_socket, 5);
    while (1)
    {
    std::cout << "Server is listening on port 8080..." << std::endl;

    client_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_length);

    char buffer[1024] = {0};
    read(client_socket, buffer, sizeof(buffer));
    std::cout << "Received: " << buffer << std::endl;

    send(client_socket, buffer, strlen(buffer), 0);
    }
    close(client_socket);
    close(server_socket);

    return 0;
}