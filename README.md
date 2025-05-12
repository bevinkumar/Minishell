# MiniShell

A custom Unix-like shell implemented in C with support for internal and external commands, piping, signal handling, and basic job tracking using data structures.

## Features

- Supports built-in commands like `cd`, `echo`, and others.
- Executes external programs using `fork()` and `execvp()`.
- Maintains a linked list of executed commands with their process IDs.
- Provides colored prompt and command-line interface (`minishell$`).
- Handles piping and background processes.
- Gracefully handles termination and interruption signals (e.g., Ctrl+C).

## File Structure

- `main.c` – Initializes the shell and starts the prompt.
- `main.h` – Header file with data structures and function declarations.
- `insert_last.c` – Linked list operations to manage command history.
- `input.c` – Functions for input parsing and execution (in your cmd.txt and input.c).
- `cmd.txt` – List of supported commands or documentation.

## Data Structures

- **Linked List**: Stores command history with process ID and status.
- **Arrays**: Used for parsing and executing command tokens.

## Compilation

```bash
gcc main.c insert_last.c input.c -o minishell
