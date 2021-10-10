# OSN Assignment 2

HuSH - A Shell written in C with Linux system calls.

## Launch HuSH

Go into the directory where hush.c is present

```
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
- Upon recieving the input, the input is first processed (in `process_input()`)to identify `&` and `;` in `utils.c`.
- In `utils.c` he sanitized commands are given to `handle_pipes()` function which handles the pipings by directing the output of first part as input to the next part by modifying I/O stream temporarily.
- `handle_pipes()` acts as a wrapper function to handle redirection and execution.
- `execute_command()` function contains a set of `if-else` statements to identify the command to be executed and calls the respective command function all of which are present in `commands` folder .
- Error handeling has been done in respective `.c` files for each command.

## Requirements met for the Assignment

All the features impemented below are as per the assignment pdf.

Assignment 1 specifications met

- Display requirement (showing prompt and path to cwd wrt home dir)
- Builtin commands (echo,pwd,cd)
- ls command
- System commands with and without arguments
- pinfo command
- Finished Background Processes
- repeat command
- Bonus 1: history
- Bonus 2.a: history <num>

Assignment 2 specifications met

- Input/Output Redirection
- Command pipelines
- I/O Redirection within Command pipelines
- User-defined Commands [ jobs sig fg bg ]
- Signal Handling [ CTRL-Z/C/D ]
- Bonus: replay

## Commands implemented

- `cd --path_to_directory` Changes the current directory to the one mentioned in the input, goes to home directory if no path is given
- `pwd` Gives the absolute path of the current directory
- `echo -string_to_be_echoed` Prints whatever is given to it to STDOUT but removing redundant spaces and tabs
- `ls -a/l --path` list the details of entities (depending on the -l or -a flag) present in the path given, if no path is given , just lists entities in current dir
- `pinfo --pid` list the process information -pid, status, memory, path to executable.
- `repeat -command_to_be_repeated` repeatedly executes teh given command
- `history --num` shows the `num` latest commands used before, if `num` is not given lists last 10 commands.
- `jobs -r/s` lists all current jobs running in the background
- `sig -job_id -signal_number` Sends the signal specified by `signal_number` to job with job id `job_id`.
- `fg -job_id` . fg Brings the running or stopped background job corresponding to job number to the foreground, and changes its state to running.
- `bg -job_id` Changes the state of a stopped background job to running (in the background).
- `replay -command --command_to_be_executed -interval --time_interval -period --till_the_time_command_will_be_executed` Executes the given `command` repeatedly till `period` for every `interval` seconds.
- `exit` Exits the shell.

Other Utilities

- `CTRL-Z` Pushes any currently running foreground job into the background, and change its state from running to stopped. Does nothing if no foreground process.
- `CTRL-C` Interrupts any currently running foreground job, by sending it the SIGINT signal. Does nothing if no foreground process
- `CTRL-D` Logs out of shell
- Adding `&` will run the command mentioned before in the background.
- Piping of commands using `|` has been implemnted to imitate the actual shell (almost completely).
- I/O redirection using `<,>,>>` has been implemeted to imitate the actual shell (almost completely).
- The commands not mentioned above have been implemented can still be executed due to implementation of `execvp`, and error ahndling is done to replicate actual shell errors.
