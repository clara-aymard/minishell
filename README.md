# minishell

A small Unix shell written in C as part of the 42 curriculum.  
The goal of this project is to recreate the core behavior of `bash`:
parsing and executing simple commands, handling redirections and pipes,
managing environment variables, and reacting correctly to signals.

---

## 🧩 Features

- **Prompt & command loop**
  - Custom prompt
  - Read and execute commands in a loop until `exit`
- **Command execution**
  - Support for absolute and relative paths
- **Builtins** (implemented directly in the shell process)
  - `echo` (with `-n`)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- **Redirections**
  - Input redirection: `<`
  - Output redirection (truncate): `>`
  - Output redirection (append): `>>`
- **Pipes**
  - Multiple pipelines: `cmd1 | cmd2 | cmd3`
- **Environment variables**
  - Expansion: `$VAR`, `$?`
  - Inheritance of parent environment at startup
- **Signals**
  - `Ctrl-C` (`SIGINT`): interrupts current command, returns to prompt
  - `Ctrl-\` (`SIGQUIT`): handled like in `bash` (ignored at prompt)
  - `Ctrl-D`: exits shell when line is empty (EOF on stdin)
- **Exit status**
  - Correctly propagate exit codes of last command / pipeline
  - `$?` reflects last command’s status

---

## 🛠️ Technical Overview

- **Language:** C
- **Standard:** `-std=c99`
- **OS:** MacOs
- **Build system:** `Makefile`
- **Main dependencies:**
  - `readline` / `history`
  - Standard C library & POSIX functions (`fork`, `execve`, `pipe`, `dup2`, `waitpid`, `signal`, etc.)

---

## 👤 Author

- **Name:** Aymard Clara and Bataboudila ThomasBataboudila
- **School:** 42 Belgium
