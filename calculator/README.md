# C Calculator

A command-line calculator written in C.

The project was built to practise C programming concepts including functions, pointers, arrays, header files, modular program design, stacks, parsing, error handling and dynamic expression evaluation.

## Features

The calculator currently supports:

* Addition: `+`
* Subtraction: `-`
* Multiplication: `*`
* Division: `/`
* Powers: `^`
* Parentheses: `(` and `)`
* Unary minus
* Floating-point numbers
* Operator precedence
* Right-associative powers
* Division-by-zero detection
* Invalid input detection
* Missing-parenthesis detection

### Examples

```text
2 + 5 * 7
Result: 37.00

(2 + 3) * 4
Result: 20.00

2 ^ 3 ^ 2
Result: 512.00

-2 ^ 2
Result: -4.00

(-2) ^ 2
Result: 4.00

2 ^ -2
Result: 0.25
```

## Structure

The project is split into several source and header files.

### `main.c`

Contains the program entry point and starts the calculator.

### `calculator.c`

Contains the main calculator logic, including:

* Reading expressions
* Operator precedence
* Parentheses
* Unary operators
* Applying operators
* Calculating intermediate results

### `calculator.h`

Contains declarations and definitions used by the calculator module, including the precedence levels and calculator functions.

### `stack.c` / `stack.h`

Implements the stack used to store numerical values.

The value stack stores `double` values such as:

```text
2
3
5.5
-4
```

### `opstack.c` / `opstack.h`

Implements a separate stack for operators.

The operator stack stores characters such as:

```text
+
-
*
/
^
(
~
```

`~` is used internally to represent unary minus.

### `input.c` / `input.h`

Contains the custom character input system and number parsing.

It provides functionality for:

* Reading characters
* Pushing characters back into the input buffer
* Reading integer and floating-point values

## Building

Compile the project using:

```bash
make
```

This produces the `calculator` executable.

To run it:

```bash
./calculator
```

To remove compiled files:

```bash
make clean
```

## Design

The calculator uses two stacks:

```text
             Expression
                 │
        ┌────────┴────────┐
        │                 │
   Number/value       Operator
      stack              stack
        │                 │
     double              char
```

The value stack stores operands while the operator stack stores operators waiting to be evaluated.

Operator precedence determines when an operator is evaluated.

For example:

```text
2 + 3 * 4
```

The multiplication is evaluated before the addition:

```text
3 * 4 = 12
2 + 12 = 14
```

Powers are right-associative, so:

```text
2 ^ 3 ^ 2
```

is evaluated as:

```text
2 ^ (3 ^ 2)
```

giving:

```text
512
```

## Purpose

This project is part of my C programming practice and is intended to develop a stronger understanding of:

* C syntax and control flow
* Functions
* Pointers
* Arrays
* Header/source file separation
* Modular program design
* Stacks
* Parsing
* Operator precedence
* Error handling
* Makefiles
* Git and GitHub
