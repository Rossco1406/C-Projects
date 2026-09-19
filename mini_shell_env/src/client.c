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

    while(1){

        char message[1024];

        printf("> ");
        fgets(message, sizeof(message), stdin);

        message[strcspn(message, "\n")] = '\0';

        if (send(client_fd, message, strlen(message), 0) < 0)
        {
            perror("send");
            break;
        }

        char buffer[1024];

        ssize_t bytes_received = recv(client_fd,
                              buffer,
                              sizeof(buffer) - 1,
                              0);

        if (bytes_received < 0)
        {
            perror("recv");
            break;
        }

        if (bytes_received == 0)
        {
            printf("Server disconnected\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("%s", buffer);

    }

    close(client_fd);

return 0;
}