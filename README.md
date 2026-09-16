# C Projects

A collection of C projects built to apply and develop the concepts learned while working through *The C Programming Language* by Brian W. Kernighan and Dennis M. Ritchie (K&R).

The projects are organised into separate directories, with each project focusing on different areas of C programming.

## Projects

### 1. Calculator

A command-line calculator written in C.

**Topics:**

* Basic arithmetic: +, -, *, /
* Operator precedence
* Parentheses
* Powers using ^
* Right-associative exponentiation
* Unary minus
* Floating-point numbers
* Error handling
* Custom input handling
* Separate value and operator stacks
* Pointers

The calculator is implemented using multiple source and header files to practise modular C programming.

### 2. File Statistics

A command-line tool for analysing files and displaying statistics about their contents.

**Topics:**

* Standard input and output streams
* Strings
* Arrays
* Pointers
* Dynamic memory
* Command-line arguments


### 3. Mini Shell & Server

A small Unix-style shell with additional server functionality.

The shell allows users to execute external programs, navigate directories, redirect input and output, and connect multiple commands using pipelines.

**Topics:**

* Processes
* fork()
* execvp()
* waitpid()
* System calls
* Pipes
* Multiple pipes
* File descriptors
* Input redirection
* Output redirection
* Append redirection
* Multiple redirections
* Process management
* Error handling
* Modular program design

The project was built to develop a practical understanding of Unix process management and how a shell interacts with the operating system.

### 4. Mini Shell Environment & Server

A more advanced version of the mini shell that combines shell functionality with a restricted Unix-style environment and server functionality.

The shell will operate inside its own designated directory within the `c-projects` repository. This directory will act as the root of the shell environment.

From the user's perspective, the starting directory will behave as `/`, and the shell will not allow navigation outside of this directory. Users will be able to move through directories within the environment, but attempts to navigate above the shell's root directory will be prevented.

For example:

```text
mini-shell/
└── environment/
    ├── home/
    ├── bin/
    ├── files/
    └── ...
```

The shell will treat `environment/` as its root, meaning:

```text
environment/
    ↓
   /
```

and:

```text
environment/home/
    ↓
   /home
```

The project will also introduce server functionality, allowing the shell environment to interact with clients over a network connection.

**Topics:**

* Restricted shell environments
* Directory traversal
* Path handling
* Processes
* System calls
* Pipes
* File descriptors
* Signals
* Networking
* Sockets
* Client/server communication
* Dynamic memory
* Security considerations
* Modular program design


## Project Structure

```text
c-projects/
├── calculator/
├── file-stats/
├── mini-shell/
├── README.md
├── mini-shell-environment/
└── .gitignore
```

Each project contains its own source code, Makefile, and README where appropriate.

Projects are intended to build upon concepts introduced in previous projects, with later projects introducing more advanced Unix and networking concepts.

## Development Environment

* **Language:** C
* **Compiler:** GCC
* **Operating System:** Linux / WSL
* **Build System:** Make
* **Version Control:** Git

## Background

These projects were created after completing *The C Programming Language* (K&R) and are intended to put the concepts from the book into practice through progressively more complex programs.

The projects focus on writing C from scratch rather than relying heavily on external libraries or frameworks.

## Goals

The main goals of this repository are to:

* Develop strong practical C programming skills
* Gain experience with Unix system programming
* Understand memory management and pointers in real programs
* Become comfortable working with files and processes
* Develop experience with networking and sockets
* Build a portfolio of practical programming projects

