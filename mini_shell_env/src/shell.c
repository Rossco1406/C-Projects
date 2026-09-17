#define _XOPEN_SOURCE 700
#include "shell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <dirent.h>

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

    if (getcwd(my_shell.root, MAXDIR) == NULL)
    {
        perror("getcwd");
        return;
    }

    strcat(my_shell.root, "/env");

    strcpy(my_shell.cwd, "/");
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

int get_real_path(const char *path, char *real_path)
{
    char virtual_path[MAXDIR];
    char resolved_path[MAXDIR];

    if (path[0] == '/')
    {
        snprintf(virtual_path, MAXDIR, "%s", path);
    }
    else
    {
        if (strcmp(my_shell.cwd, "/") == 0)
        {
            snprintf(virtual_path, MAXDIR, "/%s", path);
        }
        else
        {
            snprintf(virtual_path, MAXDIR, "%s/%s",
                     my_shell.cwd, path);
        }
    }

    if (realpath(my_shell.root, resolved_path) == NULL)
    {
        return -1;
    }

    char full_path[MAXDIR];

    snprintf(full_path, MAXDIR, "%s%s",
             resolved_path, virtual_path);

    if (realpath(full_path, real_path) == NULL)
    {
        return -1;
    }

    size_t root_length = strlen(resolved_path);

    if (strncmp(real_path, resolved_path, root_length) != 0 ||
        (real_path[root_length] != '\0' &&
         real_path[root_length] != '/'))
    {
        return -1;
    }

    return 0;
}

int get_real_output_path(const char *path, char *real_path)
{
    char virtual_path[MAXDIR];
    char resolved_root[MAXDIR];
    char full_path[MAXDIR];
    char parent_path[MAXDIR];
    char filename[MAXDIR];

    if (path[0] == '/')
    {
        snprintf(virtual_path, MAXDIR, "%s", path);
    }
    else
    {
        if (strcmp(my_shell.cwd, "/") == 0)
        {
            snprintf(virtual_path, MAXDIR, "/%s", path);
        }
        else
        {
            snprintf(virtual_path, MAXDIR, "%s/%s",
                     my_shell.cwd, path);
        }
    }

    if (realpath(my_shell.root, resolved_root) == NULL)
    {
        return -1;
    }

    snprintf(full_path, MAXDIR, "%s%s",
             resolved_root, virtual_path);

    strcpy(parent_path, full_path);

    char *last_slash = strrchr(parent_path, '/');

    if (last_slash == NULL)
    {
        return -1;
    }

    strcpy(filename, last_slash + 1);

    *last_slash = '\0';

    char resolved_parent[MAXDIR];

    if (realpath(parent_path, resolved_parent) == NULL)
    {
        return -1;
    }

    size_t root_length = strlen(resolved_root);

    if (strncmp(resolved_parent, resolved_root, root_length) != 0 ||
        (resolved_parent[root_length] != '\0' &&
         resolved_parent[root_length] != '/'))
    {
        return -1;
    }

    snprintf(real_path, MAXDIR, "%s/%s",
             resolved_parent, filename);

    return 0;
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

    if (strcmp(my_shell.argv[0], "pwd") == 0)
    {
        printf("%s\n", my_shell.cwd);
        return;
    }

    if (strcmp(my_shell.argv[0], "ls") == 0)
    {
        handle_ls();
        return;
    }

    if (strcmp(my_shell.argv[0], "mkdir") == 0)
    {
        handle_mkdir();
        return;
    }
    if (strcmp(my_shell.argv[0], "touch") == 0)
    {
        handle_touch();
        return;
    }
    if (strcmp(my_shell.argv[0], "rm") == 0)
    {
        handle_rm();
        return;
    }
    if (strcmp(my_shell.argv[0], "rmdir") == 0)
    {
        handle_rmdir();
        return;
    }

    if (find_pipe() != -1)
    {
        execute_pipes();
        return;
    }

    execute_external_command();
}

void handle_cd()
{
    if (my_shell.argv[1] == NULL)
    {
        strcpy(my_shell.cwd, "/home");

        char real_path[MAXDIR];

        if (get_real_path("/home", real_path) != 0)
        {
         fprintf(stderr, "cd: /home does not exist\n");
            return;
        }

        if (chdir(real_path) != 0)
        {
            perror("cd");
            return;
        }

        return;
    }

    if (strcmp(my_shell.cwd, "/") == 0 && strcmp(my_shell.argv[1], "..") == 0)
    {
        return;
    }

    char real_path[MAXDIR];
    char new_cwd[MAXDIR];

    if (get_real_path(my_shell.argv[1], real_path) != 0)
    {
        fprintf(stderr, "cd: path outside environment or does not exist\n");
        return;
    }

    normalise_path(
        my_shell.argv[1],
        new_cwd
    );

    if (chdir(real_path) != 0)
    {
        perror("cd");
        return;
    }

    if (my_shell.argv[1][0] == '/')
    {
        strcpy(my_shell.cwd, new_cwd);
    }
    else
    {
        char combined[MAXDIR];

        if (strcmp(my_shell.cwd, "/") == 0)
        {
            snprintf(combined, MAXDIR, "/%s",
                     my_shell.argv[1]);
        }
        else
        {
            snprintf(combined, MAXDIR, "%s/%s",
                     my_shell.cwd,
                     my_shell.argv[1]);
        }

        normalise_path(combined, my_shell.cwd);
    }
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

        char *real_argv[MAXARGS];
        char paths[MAXARGS][MAXDIR];

        for (int i = 0; i < my_shell.argc; i++)
        {
            real_argv[i] = my_shell.argv[i];

            if (i > 0 && (my_shell.argv[i][0] == '/' || my_shell.argv[i][0] == '.'))
            {
                if (get_real_path(my_shell.argv[i], paths[i]) != 0)
                {
                    fprintf(stderr, "path outside environment or does not exist\n");
                    exit(1);
                }
            real_argv[i] = paths[i];
            }
        }

        real_argv[my_shell.argc] = NULL;

        execvp(real_argv[0], real_argv);

        perror("execvp");
        exit(1);
    }

    waitpid(pid, NULL, 0);
}

void normalise_path(const char *path, char *normalised)
{
    char temp[MAXDIR];
    char *part;
    char *saveptr;
    char result[MAXDIR] = "";

    strcpy(temp, path);

    part = strtok_r(temp, "/", &saveptr);

    while (part != NULL)
    {
        if (strcmp(part, ".") == 0)
        {
            part = strtok_r(NULL, "/", &saveptr);
            continue;
        }

        if (strcmp(part, "..") == 0)
        {
            char *last_slash = strrchr(result, '/');

            if (last_slash != NULL)
            {
                *last_slash = '\0';
            }

            part = strtok_r(NULL, "/", &saveptr);
            continue;
        }

        if (strlen(result) == 0)
        {
            snprintf(result, MAXDIR, "/%s", part);
        }
        else
        {
            char new_result[MAXDIR];

            snprintf(new_result, MAXDIR, "%s/%s", result, part);
            strcpy(result, new_result);
        }

        part = strtok_r(NULL, "/", &saveptr);
    }

    if (strlen(result) == 0)
    {
        strcpy(normalised, "/");
    }
    else
    {
        strcpy(normalised, result);
    }
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

void execute_pipes()
{
    int command_count = 1;

    for (int i = 0; i < my_shell.argc; i++)
    {
        if (strcmp(my_shell.argv[i], "|") == 0)
        {
            command_count++;
        }
    }

    int pipe_count = command_count - 1;

    int pipes[pipe_count][2];
    pid_t pids[command_count];

    for (int i = 0; i < pipe_count; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            return;
        }
    }

    char **commands[command_count];

    int command_index = 0;
    commands[command_index] = my_shell.argv;

    for (int i = 0; i < my_shell.argc; i++)
    {
        if (strcmp(my_shell.argv[i], "|") == 0)
        {
            my_shell.argv[i] = NULL;

            command_index++;
            commands[command_index] = &my_shell.argv[i + 1];
        }
    }

    for (int i = 0; i < command_count; i++)
    {
        pids[i] = fork();

        if (pids[i] < 0)
        {
            perror("fork");
            return;
        }

        if (pids[i] == 0)
        {
            if (i > 0)
            {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
            }

            if (i < command_count - 1)
            {
                if (dup2(pipes[i][1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    exit(1);
                }
            }

            for (int j = 0; j < pipe_count; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            char *real_argv[MAXARGS];
            char paths[MAXARGS][MAXDIR];

            int j;

            for (j = 0; commands[i][j] != NULL; j++)
            {
                real_argv[j] = commands[i][j];

                if (j > 0 && (commands[i][j][0] == '/' || commands[i][j][0] == '.'))
                {
                    if (get_real_path(commands[i][j], paths[j]) != 0)
                    {
                        fprintf(stderr, "path outside environment or does not exist\n");
                        exit(1);
                    }
                real_argv[j] = paths[j];
                }   
            }

            real_argv[j] = NULL;

            execvp(real_argv[0], real_argv);

            perror("execvp");
            exit(1);
        }
    }

    for (int i = 0; i < pipe_count; i++)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    for (int i = 0; i < command_count; i++)
    {
        waitpid(pids[i], NULL, 0);
    }
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

            char real_path[MAXDIR];

            if (get_real_output_path(my_shell.argv[i + 1], real_path) != 0)
            {
                fprintf(stderr, "redirection: path outside environment\n");
                exit(1);
            }

            int fd = open(real_path, O_WRONLY | O_CREAT |O_TRUNC, 0644);

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

            char real_path[MAXDIR];

            if (get_real_output_path(my_shell.argv[i + 1], real_path) != 0)
            {
                fprintf(stderr, "redirection: path outside environment\n");
                exit(1);
            }

            int fd = open(real_path,O_WRONLY | O_CREAT |O_APPEND,0644);

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

            char real_path[MAXDIR];

            if (get_real_path(my_shell.argv[i + 1], real_path) != 0)
            {
                fprintf(stderr, "redirection: path outside environment or does not exist\n");
                exit(1);
            }

            int fd = open(real_path, O_RDONLY);

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

void handle_ls()
{
    char real_path[MAXDIR];

    if (my_shell.argv[1] == NULL)
    {
        strcpy(real_path, my_shell.root);
    }
    else
    {
        if (get_real_path(my_shell.argv[1], real_path) != 0)
        {
            fprintf(stderr, "ls: path outside environment or does not exist\n");
            return;
        }
    }

    DIR *dir = opendir(real_path);

    if (dir == NULL)
    {
        perror("ls");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        printf("%s  ", entry->d_name);
    }

    printf("\n");

    closedir(dir);
}

void handle_mkdir()
{
    if (my_shell.argv[1] == NULL)
    {
        fprintf(stderr, "mkdir: missing operand\n");
        return;
    }

    char real_path[MAXDIR];

    if (get_real_output_path(my_shell.argv[1], real_path) != 0)
    {
        fprintf(stderr, "mkdir: path outside environment or parent does not exist\n");
        return;
    }

    if (mkdir(real_path, 0755) != 0)
    {
        perror("mkdir");
        return;
    }
}

void handle_touch()
{
    if (my_shell.argv[1] == NULL)
    {
        fprintf(stderr, "touch: missing operand\n");
        return;
    }

    char real_path[MAXDIR];

    if (get_real_output_path(my_shell.argv[1], real_path) != 0)
    {
        fprintf(stderr, "touch: path outside environment or parent does not exist\n");
        return;
    }

    int fd = open(real_path, O_WRONLY | O_CREAT, 0644);

    if (fd < 0)
    {
        perror("touch");
        return;
    }

    close(fd);
}

void handle_rm()
{
    if (my_shell.argv[1] == NULL)
    {
        fprintf(stderr, "rm: missing operand\n");
        return;
    }

    char real_path[MAXDIR];

    if (get_real_path(my_shell.argv[1], real_path) != 0)
    {
        fprintf(stderr, "rm: path outside environment or does not exist\n");
        return;
    }

    if (remove(real_path) != 0)
    {
        perror("rm");
        return;
    }
}

void handle_rmdir()
{
    if (my_shell.argv[1] == NULL)
    {
        fprintf(stderr, "rmdir: missing operand\n");
        return;
    }

    char real_path[MAXDIR];

    if (get_real_path(my_shell.argv[1], real_path) != 0)
    {
        fprintf(stderr, "rmdir: path outside environment or does not exist\n");
        return;
    }

    if (rmdir(real_path) != 0)
    {
        perror("rmdir");
        return;
    }
}