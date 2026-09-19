#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int client_fd;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (client_fd < 0)
    {
        perror("socket");
        return 1;
    }

    printf("Socket created\n");

    struct sockaddr_in server_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_fd,
                (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0)
    {
        perror("connect");
        close(client_fd);
        return 1;
    }

    printf("Connected to server\n");

    char message[] = "Hello from client";

    if (send(client_fd, message, strlen(message), 0) < 0)
    {
        perror("send");
        close(client_fd);
        return 1;
    }

    printf("Message sent\n");

    char buffer[1024];

    ssize_t bytes_received = recv(client_fd,
                              buffer,
                              sizeof(buffer) - 1,
                              0);

    if (bytes_received < 0)
    {
        perror("recv");
        close(client_fd);
        return 1;
    }

    buffer[bytes_received] = '\0';

    printf("Server response: %s\n", buffer);

    close(client_fd);

    return 0;
}