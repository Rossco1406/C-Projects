#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int server_fd;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("socket");
        return 1;
    }

    printf("Socket created\n");

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_fd,
             (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    printf("Socket bound to port 8080\n");

    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port 8080\n");

    int client_fd;

    client_fd = accept(server_fd, NULL, NULL);

    if (client_fd < 0)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }

    printf("Client connected\n");

    char buffer[1024];

    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received < 0)
    {
        perror("recv");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';

    printf("Received: %s\n", buffer);

    char response[] = "Hello from server";

    if (send(client_fd, response, strlen(response), 0) < 0)
    {
        perror("send");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    printf("Response sent\n");

    close(client_fd);
    close(server_fd);

    return 0;
}

