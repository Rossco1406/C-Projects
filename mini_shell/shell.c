#include "shell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

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

void execute_command(){

    if (my_shell.argv[0] == NULL)
    {
        return;
    }

    if (strcmp(my_shell.argv[0],"exit")== 0){
        exit(0);
    }

    if (strcmp(my_shell.argv[0],"cd") == 0 ){
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
        return;
    }

    pid_t p = fork();
    if(p<0){
      perror("fork fail");
      exit(1);
    }
    else if (p == 0){
        execvp(my_shell.argv[0], my_shell.argv);

        perror("execvp");
        exit(1);
    }
    else{
        waitpid(p, NULL, 0);
    }


}