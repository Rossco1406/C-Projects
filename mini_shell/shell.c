#include "shell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>

struct shell my_shell;

void shell_run(void)
{
    init_shell();
    while (1)
    {
        print_prompt();
        read_input();
        parse_input();
        execute_command();
    }
}

void init_shell(){
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    
    if (pw == NULL)
    {
        fprintf(stderr, "Failed to get username\n");
        return;
    }
    strcpy(my_shell.user, pw->pw_name);

    get_cwd();
}

void print_prompt(){
    printf("%s: %s $ ", my_shell.user, my_shell.cwd);
}

void read_input(void)
{
    if (fgets(my_shell.input, sizeof(my_shell.input), stdin) == NULL)
        exit(0);
}

void get_cwd(void)
{
    if (getcwd(my_shell.cwd, MAXDIR) == NULL)
    {
        perror("getcwd");
    }
}

void execute_command()
{
    if (my_shell.argv[0] == NULL)
        return;

    if (strcmp(my_shell.argv[0], "exit") == 0)
    {
        exit(0);
    }

    if (strcmp(my_shell.argv[0], "cd") == 0)
    {
        handle_cd();
        return;
    }

    int pipe_pos = find_pipe();

    if (pipe_pos != -1)
    {
        execute_pipe(pipe_pos);
        return;
    }

    execute_external_command();
}

void handle_cd(){
    if (my_shell.argv[1] == NULL)
        {
            char *home = getenv("HOME");
            if (home == NULL || chdir(home) != 0)
                perror("cd");
        }
        else if (chdir(my_shell.argv[1]) != 0)
        {
            perror("cd");
        }

        get_cwd();
}

void execute_external_command()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        handle_redirections();

        execvp(my_shell.argv[0], my_shell.argv);

        perror("execvp");
        exit(1);
    }

    waitpid(pid, NULL, 0);
}


int find_pipe()
{
    for (int i = 0; i < my_shell.argc; i++)
    {
        if (strcmp(my_shell.argv[i], "|") == 0)
        {
            return i;
        }
    }

    return -1;
}

void execute_pipe(int pipe_pos)
{
    int pipefd[2];

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        return;
    }

    char **left_argv = my_shell.argv;
    char **right_argv = &my_shell.argv[pipe_pos + 1];

    my_shell.argv[pipe_pos] = NULL;

    pid_t left_pid = fork();

    if (left_pid < 0)
    {
        perror("fork");
        return;
    }

    if (left_pid == 0)
    {
        close(pipefd[0]);

        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }

        close(pipefd[1]);

        execvp(left_argv[0], left_argv);

        perror("execvp");
        exit(1);
    }

    pid_t right_pid = fork();

    if (right_pid < 0)
    {
        perror("fork");
        return;
    }

    if (right_pid == 0)
    {
        close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(1);
        }

        close(pipefd[0]);

        execvp(right_argv[0], right_argv);

        perror("execvp");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(left_pid, NULL, 0);
    waitpid(right_pid, NULL, 0);
}

void handle_redirections()
{
    for (int i = 0; i < my_shell.argc; i++)
    {
        if (strcmp(my_shell.argv[i], ">") == 0)
        {
            if (i + 1 >= my_shell.argc)
            {
                fprintf(stderr, "No filename\n");
                exit(1);
            }

            int fd = open(my_shell.argv[i + 1],
                          O_WRONLY | O_CREAT | O_TRUNC,
                          0644);

            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }

            close(fd);

            remove_redirection(i);
            i--;
        }

        else if (strcmp(my_shell.argv[i], ">>") == 0)
        {
            if (i + 1 >= my_shell.argc)
            {
                fprintf(stderr, "No filename\n");
                exit(1);
            }

            int fd = open(my_shell.argv[i + 1],
                          O_WRONLY | O_CREAT | O_APPEND,
                          0644);

            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }

            close(fd);

            remove_redirection(i);
            i--;
        }

        else if (strcmp(my_shell.argv[i], "<") == 0)
        {
            if (i + 1 >= my_shell.argc)
            {
                fprintf(stderr, "No filename\n");
                exit(1);
            }

            int fd = open(my_shell.argv[i + 1], O_RDONLY);

            if (fd < 0)
            {
                perror("open");
                exit(1);
            }

            if (dup2(fd, STDIN_FILENO) < 0)
            {
                perror("dup2");
                close(fd);
                exit(1);
            }

            close(fd);

            remove_redirection(i);
            i--;
        }
    }
}

void remove_redirection(int position)
{
    for (int j = position; j < my_shell.argc - 2; j++)
    {
        my_shell.argv[j] = my_shell.argv[j + 2];
    }

    my_shell.argc -= 2;
    my_shell.argv[my_shell.argc] = NULL;
}

