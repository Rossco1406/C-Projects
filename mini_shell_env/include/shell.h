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
    char root[MAXDIR];
};

extern struct shell my_shell;

void init_shell();
void shell_run(void);
int get_real_path(const char *path, char *real_path);
int get_real_output_path(const char *path, char *real_path);
int change_directory(const char *path);
void get_cwd(void);
void handle_cd(void);
void normalise_path(const char *path, char *normalised);
void print_prompt(void);
void read_input(void);
void execute_command(void);
void execute_external_command(void);
void remove_redirection(int position);
void handle_redirections(void);
int find_pipe(void);
void execute_pipes(void);
void handle_ls(void);
void handle_mkdir(void);
void handle_touch(void);
void handle_rm(void);
void handle_rmdir(void);
void handle_pipe_redirections(char **argv);
void execute_command_string(const char *command);


#endif