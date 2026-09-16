# Mini Shell

A simple command-line shell written in C that allows users to execute commands, navigate directories, redirect input and output, and connect commands using pipelines.

The project was built to practise C programming concepts including processes, `fork()`, `execvp()`, pipes, file descriptors, input/output redirection, functions, arrays, pointers, header files, modular program design, error handling, Makefiles and Linux system calls.

## Features

Supports built-in commands and external programs, alongside input/output redirection and multiple command pipelines.

The shell currently supports:

* `cd`
* `exit`
* External commands
* Input redirection `<`
* Output redirection `>`
* Append redirection `>>`
* Multiple redirections
* Single pipes
* Multiple pipes
* Error handling for invalid commands and file operations

### Examples

Run the shell:

```bash
./minishell
```

Execute a command:

```bash
ls
```

Change directory:

```bash
cd ..
```

Input redirection:

```bash
cat < input.txt
```

Output redirection:

```bash
echo Hello > output.txt
```

Append output:

```bash
echo World >> output.txt
```

Multiple redirections:

```bash
cat < input.txt > output.txt
```

Pipelines:

```bash
ls | grep .c
```

Multiple pipelines:

```bash
ls | grep .c | wc -l
```

## Structure

The project is split into multiple source and header files to keep different parts of the shell modular.

### `main.c`

Contains the program entry point and starts the shell.

### `shell.c`

Contains the main shell logic, including:

* Shell initialisation
* User and current directory information
* Command execution
* Built-in commands
* External command execution
* Pipes
* Input/output redirection
* Process creation and management

### `shell.h`

Contains the shell structure, constants and function prototypes used throughout the project.

### `parser.c`

Contains the command parsing logic used to process user input and create the argument array passed to the shell.

### `parser.h`

Contains the parser function prototypes and definitions required by the parser.

## Building

Compile the project using:

```bash
make
```

This produces the `minishell` executable.

To run it:

```bash
./minishell
```

The shell will display a prompt containing the current username and working directory.

To remove compiled files:

```bash
make clean
```

## Purpose

This project is part of my C programming practice and is intended to develop a stronger understanding of:

* C syntax and control flow
* Functions
* Pointers
* Arrays
* Structures
* Header/source file separation
* Modular program design
* Command-line arguments
* Processes
* `fork()`
* `execvp()`
* `waitpid()`
* Pipes
* File descriptors
* `open()`
* `dup2()`
* Standard input and output streams
* Linux system calls
* Error handling
* Makefiles
* Git and GitHub
