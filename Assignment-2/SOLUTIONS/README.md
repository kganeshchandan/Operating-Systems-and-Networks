# OSN Assignment 2

HuSH - A Shell written in C with Linux system calls.

## Launch HuSH

Go into the directory where hush.c is present

```sh
make # will create the executable file HuSH by compiling all the c files
./HuSH # launches the shell
```

## File Structure

The Directory has the followin files or directories

- `utils.c` - contains all utiliy functions
- `makefile` - compiles the codebase
- `utils.h`- header file for all utility functions
- `hush.c` - driver code
- `commands` - directry contains all header files and `.c` files for all implemented commands for this shell
- `README.md` - The file ur looking at right now

## Working of HuSH

`HuSH` executable launches the shell upon execution
The Shell assumes the directory from where it was launched as the home directory.
All the relative paths presented in the shell are formatted assuming the launch directory of shell as home directory.
`hush.c` is the driver code for the shell.

- `main` function first initialises the launch dir as home dir, initialises history file if it doesnt exist and calls `launch_hush()`
- `launch_hush()` runs a while loop which generates prompt continuously and waits for the users input.
- Upon recieving the input, the input is first processed (in `process_input()`)to identify `&` and `;`.
- The sanitized commands are given to `execute_commnad()` to execute the command by running a set of `if -else` statements to identify the command and call the respective command (present in `commands/command.c`).

## Requirements met for the Assignment

All the features impemented below are as per the assignment pdf.

- 1: Display requirement (showing prompt and path to cwd wrt home dir)
- 2: Builtin commands (echo,pwd,cd)
- 3: ls command
- 4: System commands with and without arguments
- 5: pinfo command
- 6: Finished Background Processes
- 7: repeat command
- Bonus 1: history
- Bonus 2.a: history <num>
