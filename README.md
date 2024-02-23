# Linux-Shell
Linux Shell

Youtube Video link https://youtu.be/p4SxIsuDCMs

ssh ameyasantoshgidh@login.ccs.neu.edu

## How to Run this program locally

1. Install Clang compiler
2. cd Assignment_6
3. cd part1
4. clang -o prog shell.c
5. Congratulations you are now in the minishell

## Introduction

I've developed a Mini Shell in C, a command-line interface inspired by popular shells like Bash. This Mini Shell project serves as a bridge between users and the operating system, allowing them to execute various commands. It's a versatile and expandable project that can be used as a foundation for further development.

## Images

<img width="568" alt="Screenshot 2024-02-23 at 2 22 02 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/9017a528-a24b-4e88-9990-e0edba185093">

#### Clear command
<img width="572" alt="Screenshot 2024-02-23 at 2 14 05 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/606a2056-e7b8-4717-915e-db117f5a287f">

#### cat command 
<img width="564" alt="Screenshot 2024-02-23 at 2 14 28 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/50b287c9-aca5-4f4d-a7df-602a2b7cdc13">

#### history command shows the commands used
<img width="568" alt="Screenshot 2024-02-23 at 2 16 23 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/e603d8d8-663f-4d92-a27a-97840dbb74e7">

#### nl command counts the number of lines in the file shell.c
<img width="570" alt="Screenshot 2024-02-23 at 2 16 53 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/ab6a0eed-7c23-463e-bbb3-04bf27eaec47">

#### exit
<img width="566" alt="Screenshot 2024-02-23 at 2 21 17 AM" src="https://github.com/ameyagidh/Linux-Shell/assets/65457905/4e637d5e-6df9-4faf-a91e-33527b9af915">

## Features

Here's an overview of the key features of this Mini Shell:

### Task 1 - Signals

I've implemented a signal handler in the shell, allowing it to respond to signals sent to processes. Signal handlers are essential for managing processes and handling events in the operating system. For instance, pressing `Ctrl+C` sends a signal that can be caught and handled by the shell's signal handler.

### Task 2 - Mini-Shell

This Mini Shell project is designed to provide a user-friendly command-line interface with several important features:

1. **Prompt**: The shell displays the prompt "mini-shell>" before each command, providing a clear indication of where user input is expected.

2. **Input Limit**: The maximum input size is set to 80 characters per line, ensuring efficient parsing and execution.

3. **Signal Handling**: I've implemented a signal handler that responds to `Ctrl+C`. When the user presses `Ctrl+C`, the shell displays "mini-shell terminated" and ensures that all child processes are terminated as well.

4. **Foreground Execution**: By default, child processes run in the foreground until their execution is completed. This behavior mimics typical shell behavior.

5. **Pipes**: The shell supports pipes (`|`) for chaining commands together. Users can create pipelines to process data through a series of commands.

6. **Built-in Commands**:
   - `cd`: Change the current working directory.
   - `help`: Display information about available built-in commands.
   - `exit`: Terminate the shell.
   - `history`: Keep track of user input commands and display them upon request.

7. **Error Handling**: The shell provides informative error messages for command not found and offers suggestions for similar commands.

## Usage

To use this Mini Shell:

1. Compile the shell program.
   
   ```bash
   gcc mini-shell.c -o mini-shell
   ```

2. Run the shell.

   ```bash
   ./mini-shell
   ```

3. Start typing commands with the "mini-shell>" prompt.

You can test various commands, including built-in commands like `cd`, `help`, `exit`, and the custom `history` command. Experiment with pipes to chain commands together and explore the shell's functionality.
