# Mini Shell Environment

A Linux-based command shell written in C, extended with a virtual filesystem environment and a TCP server allowing remote clients to interact with the shell over a network.

The project was developed as a progression from a basic command-line shell into a small systems-programming and networking project.

## Features

### Shell

* Built-in commands:

  * `cd`
  * `pwd`
  * `ls`
  * `mkdir`
  * `touch`
  * `rm`
  * `rmdir`
  * `exit`
* External command execution using `fork()` and `execvp()`
* Input and output redirection:

  * `<`
  * `>`
  * `>>`
* Multiple redirections
* Single and multiple pipes using `|`
* Process and file-descriptor management

### Virtual Filesystem

The shell operates inside a restricted virtual environment:

```text
env/
├── home/
├── files/
└── ...
```

The `env/` directory acts as the virtual root `/`.

For example:

```text
/
├── home
├── files
└── ...
```

A command such as:

```bash
cd /home
```

changes the shell's virtual working directory without exposing the real host filesystem.

Path validation prevents commands from escaping the virtual environment through paths such as:

```bash
cd ..
cat ../../etc/passwd
```

### Networking

The shell was extended with a TCP server and client.

The server:

* Creates a TCP socket
* Binds to port `8080`
* Accepts client connections
* Creates a child process for each client using `fork()`
* Receives shell commands
* Executes them using the existing shell
* Captures command output
* Sends the output back to the client

Multiple clients can connect simultaneously.

The client provides an interactive command prompt and communicates with the server over TCP.

## Architecture

```text
                    TCP
┌──────────────┐              ┌──────────────────────┐
│    Client    │─────────────▶│       Server         │
│              │              │                      │
│  iPhone /    │              │  accept()            │
│  Linux/etc.  │◀─────────────│  fork()              │
└──────────────┘              │  recv() / send()     │
                              └──────────┬───────────┘
                                         │
                                         ▼
                              ┌──────────────────────┐
                              │     Shell Engine     │
                              │                      │
                              │ parser.c             │
                              │ shell.c              │
                              │                      │
                              │ cd / ls / pwd / ...  │
                              │ pipes / redirection  │
                              └──────────┬───────────┘
                                         │
                                         ▼
                              ┌──────────────────────┐
                              │  Virtual Filesystem  │
                              │                      │
                              │       env/           │
                              └──────────────────────┘
```

## Project Structure

```text
mini_shell_env/
├── src/
│   ├── main.c
│   ├── shell.c
│   ├── parser.c
│   ├── server.c
│   └── client.c
│
├── include/
│   ├── shell.h
│   └── parser.h
│
├── env/
│   ├── home/
│   ├── files/
│   └── ...
│
├── Makefile
└── README.md
```

## Building

The project is designed to run on Linux/WSL.

Compile the shell:

```bash
make
```

The server can currently be compiled with:

```bash
gcc -Wall -Wextra -std=c11 -Iinclude \
    src/server.c src/shell.c src/parser.c \
    -o server
```

Compile the client with:

```bash
gcc -Wall -Wextra -std=c11 \
    src/client.c \
    -o client
```

## Running the Local Shell

From the project directory:

```bash
./minishell
```

The `env/` directory becomes the virtual root of the shell.

Example:

```text
/ $ pwd
/

/ $ ls
files home

/ $ cd home

/home $ pwd
/home
```

## Running the Server

Start the server:

```bash
./server
```

The server listens on TCP port `8080`.

A local client can then connect to:

```text
127.0.0.1:8080
```

Commands can be entered through the client:

```text
> pwd
/
> ls
files
home
> mkdir test
> ls
files
home
test
```

## Remote Testing

The server was also tested from another device on the same local network.

The TCP connection follows:

```text
Client
   │
   ▼
Windows host :8080
   │
   ▼
WSL
   │
   ▼
C TCP server
```

This allows a device such as an iPhone to send commands to the C server over Wi-Fi.

## Concurrency

The server uses `fork()` to create a separate child process for each connected client.

The parent process continues accepting new connections while each child handles its client.

```text
                Server
                  │
          ┌───────┴───────┐
          │               │
       Client 1        Client 2
          │               │
       Child 1          Child 2
```

Child processes are cleaned up using `SIGCHLD` handling to prevent zombie processes.

## Security

The virtual filesystem uses path validation to prevent access outside the `env/` directory.

Examples of rejected operations include:

```bash
cat /etc/passwd
```

and:

```bash
cat ../../etc/passwd
```

from inside the virtual environment.

Output redirection is also checked so that commands cannot create files outside the virtual filesystem.

The project does not use `chroot()`. Instead, paths are translated and validated by the shell before filesystem operations are performed.

## Systems Programming Concepts

This project provided practical experience with:

* C
* Linux
* POSIX APIs
* Processes
* `fork()`
* `execvp()`
* `waitpid()`
* Signals
* File descriptors
* `pipe()`
* `dup()`
* `dup2()`
* Filesystem APIs
* Directory traversal
* Path validation
* TCP sockets
* Client/server architecture
* Concurrent connections
* Makefiles
* Git

## Future Improvements

Potential future development includes:

* Robust TCP message framing
* Handling partial `send()` and `recv()` operations
* Support for command output larger than the current buffer
* Sending `stderr` to the client
* Improving command and input length handling
* More complete shell syntax
* Better client/server error handling
* Authentication
* A more structured networking protocol
* Moving server/client compilation into the Makefile

## What I Learned

The project developed from implementing basic shell functionality into a larger systems-programming project involving processes, filesystems and networking.

The most significant progression was connecting several independent concepts:

```text
Shell
  ↓
Processes
  ↓
Virtual Filesystem
  ↓
TCP Server
  ↓
Concurrent Clients
```

This provided practical experience with how Linux processes, file descriptors, system calls and network sockets interact at a low level.

