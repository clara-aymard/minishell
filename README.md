# Minishell

A Unix shell written in C, implementing the core mechanisms behind command interpretation and process execution on Linux.

This project was developed as part of the 42 curriculum and focuses on low-level Unix concepts such as process creation, inter-process communication, file descriptor management, signal handling, and environment management.

## Overview

Minishell reproduces a subset of Bash behavior while remaining compliant with the project specifications.

Supported features include:

* Interactive command prompt
* Command execution through PATH resolution
* Builtins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
* Input/output redirections (`<`, `>`, `>>`)
* Here-documents (`<<`)
* Pipelines of arbitrary length
* Environment variable expansion (`$VAR`, `$?`)
* Signal handling (`SIGINT`, `SIGQUIT`, EOF)
* Exit status propagation

## Technical Highlights

### Process Management

* Process creation using `fork()`
* Program execution using `execve()`
* Child process synchronization using `waitpid()`
* Exit status propagation

### File Descriptor Management

* Redirection handling with `dup2()`
* Pipe creation using `pipe()`
* Descriptor lifetime management
* Resource cleanup and leak prevention

### Inter-Process Communication

* Multi-stage pipelines
* Data flow through chained pipes
* Correct descriptor inheritance across processes

### Signal Handling

Interactive behavior similar to Bash:

| Signal | Behavior                                           |
| ------ | -------------------------------------------------- |
| Ctrl-C | Interrupt current command and display a new prompt |
| Ctrl-D | Exit shell                                         |
| Ctrl-\ | Ignored while waiting for commands                 |

### Environment Management

* Environment inheritance from parent process
* Dynamic variable creation and removal
* PATH resolution
* Expansion of environment variables and exit status

### Reliability

* Bash comparison testing
* Memory debugging using Valgrind
* File descriptor leak detection
* Graceful handling of invalid input and execution failures

## Personal Contributions

This project was developed by a team of two.

My primary responsibilities included:

* Command execution
* Builtin implementation
* Pipe management
* Redirections
* Here-document implementation
* Signal handling
* File descriptor management
* Exit status propagation
* Debugging and validation with Valgrind

Lexical analysis, parsing and variable expansion were primarily implemented by my teammate.

## Build

### Requirements

* Linux (tested on Ubuntu)
* GCC
* GNU Readline

### Compilation

```bash
make
```

### Run

```bash
./minishell
```

## Testing & Validation

The repository includes automated test scripts used during development.

### Functional Testing

Compare Minishell behavior against Bash:

```bash
chmod +x tests/test_minishell.sh
KEEP_TMP=1 ./tests/test_minishell.sh
```

The test suite covers:

* Command execution
* Builtins
* Redirections
* Pipes
* Here-documents
* Environment variables
* Exit statuses
* Error handling

### Memory & Resource Validation

Run automated Valgrind checks:

```bash
chmod +x tests/valgrind_minishell.sh
./tests/valgrind_minishell.sh
```

Final validation results:

* 0 bytes definitely lost
* 0 bytes indirectly lost
* 0 bytes possibly lost
* 0 memory errors reported by Valgrind

Remaining "still reachable" allocations originate from GNU Readline and terminal libraries.

## What I Learned

This project provided hands-on experience with:

* POSIX process management
* Unix file descriptor mechanics
* Inter-process communication
* Signal handling
* Shell execution models
* Resource cleanup strategies
* Low-level debugging with Valgrind and GDB

These concepts form the foundation of Linux systems programming and low-level software development.

## Authors

* Clara Aymard
* Thomas Bataboudila

42 Belgium
