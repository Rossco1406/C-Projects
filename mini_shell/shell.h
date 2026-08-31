#ifndef SHELL_H
#define SHELL_H

#define MAXUSERNAME 100
#define MAXDIR 1024
#define MAXINPUT 100
#define MAXARGS 100

struct shell{
    char user[MAXUSERNAME];
    char cwd[MAXDIR];
    char input[MAXINPUT];
    int argc;
    char *argv[MAXARGS];
};

extern struct shell my_shell;

void init_shell();
void shell_run(void);
void get_cwd(void);
void print_prompt(void);
void read_input(void);
void exectute_command(void);

#endif