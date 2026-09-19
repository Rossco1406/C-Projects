#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "shell.h"

int main()
{
    init_shell();

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

    while (1){

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

        if (strcmp(buffer, "exit") == 0)
        {
            printf("Client requested disconnect\n");
            break;
        }

        int output_pipe[2];

        if (pipe(output_pipe) < 0)
        {
            perror("pipe");
            close(client_fd);
            close(server_fd);
            return 1;
        }

        int saved_stdout = dup(STDOUT_FILENO);

        if (saved_stdout < 0)
        {
            perror("dup");
            close(output_pipe[0]);
            close(output_pipe[1]);
            close(client_fd);
            close(server_fd);
            return 1;
        }

        if (dup2(output_pipe[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(output_pipe[0]);
            close(output_pipe[1]);
            close(saved_stdout);
            close(client_fd);
            close(server_fd);
            return 1;
        }

        close(output_pipe[1]);  

        execute_command_string(buffer);

        if (dup2(saved_stdout, STDOUT_FILENO) < 0)
        {
            perror("dup2");
        }

        close(saved_stdout);

        char output[4096];

        ssize_t bytes_read = read(output_pipe[0],
                            output,
                            sizeof(output) - 1);

        if (bytes_read < 0)
        {
            perror("read");
            close(output_pipe[0]);
            close(client_fd);
            close(server_fd);
            return 1;
        }

        output[bytes_read] = '\0';

        close(output_pipe[0]);

        printf("Command output:\n%s", output);

        if (send(client_fd, output, bytes_read, 0) < 0)
        {
            perror("send");
            close(client_fd);
            close(server_fd);
            return 1;
        }

        printf("Response sent\n");
    }

    close(client_fd);
    close(server_fd);

    return 0;
}

