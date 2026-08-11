# C File Statistics

A command-line tool for analysing files and displaying statistics about their contents.

The project was built to practise C programming concepts including functions, pointers, arrays, header files, modular program design, error handling and file reading and writing.

## Features

Supports text files for line, word, and character statistics. Binary files can be read, but line and word statistics are not meaningful for binary data.  

This is the data that is currently displayed  

* Filename
* File Size
* Line count
* Word count
* Character count

### Examples

Execute command
```bash
./filestats text1.txt text2.txt
```
Contents of text1.txt
```text1.txt
Hello
World
```
Output on terminal
```bash
Statistics written to test2.txt
```
Contents of text2.txt
```text2.txt
Filename: text1.txt
File Size: 12
Lines: 2
Words: 2
Characters: 12
```

## Structure

The project is split into 2 source and a header file.

### `main.c`

Contains the program entry point and handles any errors with user entry and the calls on the file statistic logic.

### `filestats.c`

Contains the file statistic logic, including:

* Counting characters
* Counting words
* Counting Lines
* Counting File Size
* Printing results to destination file

## Building

Compile the project using:

```bash
make
```

This produces the `filestats` executable.

To run it:

```bash
./filestats <optinal input file> <optional output file>
```
Ensure input file exists. If no files named the input is taken from stdin and printed on stdout. If only 1 file named, the file has to exist and then the output is sent to stdout.

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
* Header/source file separation
* Modular program design
* Standard input and output streams
* Command-line arguments
* Error handling
* Makefiles
* Git and GitHub