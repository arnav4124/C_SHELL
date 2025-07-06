# C Shell - A Comprehensive Command Line Interface

A feature-rich command-line shell implementation in C that provides a complete Unix-like shell experience with advanced process management, file operations, networking capabilities, and more.

## Table of Contents

- [Features](#features)
- [Installation and Usage](#installation-and-usage)
- [Core Shell Features](#core-shell-features)
- [Built-in Commands](#built-in-commands)
- [Process Management](#process-management)
- [File Operations](#file-operations)
- [I/O Redirection and Pipes](#io-redirection-and-pipes)
- [Signal Handling](#signal-handling)
- [Networking Features](#networking-features)
- [Configuration](#configuration)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Assumptions and Limitations](#assumptions-and-limitations)

## Features

### Core Shell Features

- **Dynamic Prompt**: Displays username, system name, and current directory
- **Command Parsing**: Supports multiple commands separated by `;` and `&`
- **Background Process Management**: Run processes in background with `&`
- **Signal Handling**: Ctrl+C, Ctrl+Z, Ctrl+D support
- **Command History**: Persistent command logging across sessions

### Built-in Commands

- **hop**: Directory navigation with path support
- **reveal**: File listing with color coding and detailed information
- **seek**: File/directory search with filtering options
- **log**: Command history management
- **proclore**: Process information display
- **activities**: List running background processes
- **fg/bg**: Foreground/background process control
- **ping**: Send signals to processes
- **iman**: Fetch man pages from internet
- **neonate**: Monitor most recent process creation

### Advanced Features

- **I/O Redirection**: `>`, `>>`, `<` support
- **Pipes**: Multi-stage command pipelines
- **Aliases and Functions**: Custom command shortcuts
- **Color-coded Output**: Different colors for files, directories, executables
- **Error Handling**: Comprehensive error messages and recovery

## Installation and Usage

### Prerequisites

- GCC compiler
- Linux/Unix environment (WSL recommended for Windows)
- Standard C libraries

### Compilation

```bash
make
```

### Running the Shell

```bash
./a.out
```

### Clean Build

```bash
make clean
```

## Core Shell Features

### Dynamic Prompt

The shell displays a prompt in the format: `<Username@SystemName:CurrentDirectory>`

- **Home Directory**: Represented as `~`
- **Relative Paths**: Shown relative to home directory
- **Absolute Paths**: Full path when outside home directory
- **Process Timing**: Shows execution time for foreground processes > 2 seconds

Example:

```
<JohnDoe@SYS:~>
<JohnDoe@SYS:~/Documents>
<JohnDoe@SYS:/usr/bin>
<JohnDoe@SYS:~ sleep : 5s>
```

### Command Parsing

- **Multiple Commands**: Use `;` to separate commands
- **Background Execution**: Use `&` to run commands in background
- **Whitespace Handling**: Automatically handles extra spaces and tabs
- **Error Recovery**: Continues execution even if one command fails

Example:

```bash
<JohnDoe@SYS:~> ls ; pwd ; echo "Hello"
<JohnDoe@SYS:~> sleep 10 & echo "Background process started"
[1] 12345
Background process started
```

## Built-in Commands

### hop - Directory Navigation

Changes the current working directory with advanced path support.

**Syntax**: `hop [path]`

**Features**:

- **Relative Paths**: `hop test`, `hop ../parent`
- **Absolute Paths**: `hop /usr/bin`
- **Special Symbols**:
  - `~` - Home directory
  - `.` - Current directory
  - `..` - Parent directory
  - `-` - Previous directory
- **Multiple Arguments**: Sequential directory changes
- **Path Display**: Shows full path after successful change

**Examples**:

```bash
<JohnDoe@SYS:~> hop test
/home/johndoe/test
<JohnDoe@SYS:~/test> hop ~
/home/johndoe
<JohnDoe@SYS:~> hop .. tutorial
/home/johndoe
/home/johndoe/tutorial
```

### reveal - File Listing

Lists files and directories with detailed information and color coding.

**Syntax**: `reveal [flags] [path]`

**Flags**:

- `-a`: Show all files (including hidden)
- `-l`: Show detailed information
- `-la` or `-al`: Combine both flags

**Features**:

- **Color Coding**:
  - Green: Executables
  - Blue: Directories
  - White: Regular files
- **Detailed Information**: Permissions, owner, size, modification date
- **Lexicographic Ordering**: Sorted output
- **Path Support**: Relative, absolute, and special paths

**Examples**:

```bash
<JohnDoe@SYS:~> reveal
<JohnDoe@SYS:~> reveal -l
<JohnDoe@SYS:~> reveal -a ~/Documents
```

### seek - File Search

Searches for files and directories with advanced filtering options.

**Syntax**: `seek [flags] <search_term> [target_directory]`

**Flags**:

- `-d`: Search only directories
- `-f`: Search only files
- `-e`: Execute action on single match
  - For files: Display content
  - For directories: Change to directory

**Features**:

- **Prefix Matching**: Finds files/directories starting with search term
- **Recursive Search**: Searches entire directory tree
- **Permission Checking**: Validates access permissions
- **Relative Path Output**: Shows paths relative to search directory

**Examples**:

```bash
<JohnDoe@SYS:~> seek test
./test
./test/file.txt
<JohnDoe@SYS:~> seek -d folder
./folder
<JohnDoe@SYS:~> seek -e -f config
./config.txt
[file content displayed]
```

### log - Command History

Manages command history with persistent storage across sessions.

**Syntax**:

- `log` - Display command history
- `log purge` - Clear all history
- `log execute <index>` - Execute command by index

**Features**:

- **Persistent Storage**: Commands saved across shell sessions
- **Duplicate Prevention**: Doesn't store consecutive identical commands
- **Command Filtering**: Excludes `log` commands from history
- **Index-based Execution**: Execute previous commands by number
- **Maximum Storage**: Stores up to 15 commands (FIFO)

**Examples**:

```bash
<JohnDoe@SYS:~> log
ls
pwd
echo "Hello"
<JohnDoe@SYS:~> log execute 1
[executes "pwd"]
<JohnDoe@SYS:~> log purge
<JohnDoe@SYS:~> log
[empty]
```

### proclore - Process Information

Displays detailed information about processes.

**Syntax**: `proclore [pid]`

**Features**:

- **Self Information**: Shows shell process info if no PID provided
- **Process Details**: PID, status, group, virtual memory, executable path
- **Status Codes**: R (Running), S (Sleeping), Z (Zombie)
- **Foreground/Background**: + indicates foreground process

**Examples**:

```bash
<JohnDoe@SYS:~> proclore
pid : 210
process status : R+
Process Group : 210
Virtual memory : 167142
executable path : ~/a.out

<JohnDoe@SYS:~> proclore 1234
pid : 1234
process Status : S
Process Group : 1200
Virtual memory : 177013
executable Path : /usr/bin/sleep
```

## Process Management

### Background Processes

- **Background Execution**: Use `&` to run commands in background
- **PID Display**: Shows process ID when background process starts
- **Automatic Notification**: Notifies when background process completes
- **Multiple Background Processes**: Supports multiple concurrent background processes

**Examples**:

```bash
<JohnDoe@SYS:~> sleep 10 &
[1] 12345
<JohnDoe@SYS:~> sleep 20 &
[2] 12346
[1] sleep exited normally (12345)
```

### activities - Process Listing

Lists all background processes spawned by the shell.

**Syntax**: `activities`

**Output Format**: `[pid] : [command name] - [State]`

**States**:

- Running: Process is actively executing
- Stopped: Process is suspended

**Example**:

```bash
<JohnDoe@SYS:~> activities
221 : emacs new.txt - Running
430 : vim - Stopped
620 : gedit - Stopped
```

### fg/bg - Process Control

Control foreground and background processes.

**Syntax**:

- `fg <pid>` - Bring process to foreground
- `bg <pid>` - Resume stopped background process

**Features**:

- **Process Validation**: Checks if process exists
- **State Management**: Handles process state transitions
- **Terminal Control**: Proper terminal handoff for foreground processes

**Examples**:

```bash
<JohnDoe@SYS:~> fg 430
[brings vim to foreground]
<JohnDoe@SYS:~> bg 620
[resumes gedit in background]
```

### ping - Signal Sending

Send signals to processes using process ID and signal number.

**Syntax**: `ping <pid> <signal_number>`

**Features**:

- **Signal Validation**: Checks if process exists
- **Signal Number Handling**: Uses modulo 32 for signal mapping
- **Error Handling**: Reports non-existent processes

**Common Signals**:

- 9: SIGKILL (force kill)
- 15: SIGTERM (graceful termination)
- 2: SIGINT (interrupt)

**Examples**:

```bash
<JohnDoe@SYS:~> ping 1234 9
Sent signal 9 to process with pid 1234
<JohnDoe@SYS:~> ping 5678 15
Sent signal 15 to process with pid 5678
```

## File Operations

### I/O Redirection

Supports input/output redirection with file operations.

**Operators**:

- `>`: Output redirection (overwrite)
- `>>`: Output redirection (append)
- `<`: Input redirection

**Features**:

- **File Creation**: Creates output files with 0644 permissions
- **Error Handling**: Reports missing input files
- **Append Mode**: Preserves existing content with `>>`

**Examples**:

```bash
<JohnDoe@SYS:~> echo "Hello" > output.txt
<JohnDoe@SYS:~> cat input.txt > output.txt
<JohnDoe@SYS:~> echo "World" >> output.txt
<JohnDoe@SYS:~> wc < input.txt
```

### Pipes

Supports command pipelines for data flow between commands.

**Syntax**: `command1 | command2 | command3`

**Features**:

- **Multiple Pipes**: Supports any number of pipe stages
- **Sequential Execution**: Commands run from left to right
- **Error Handling**: Reports invalid pipe usage

**Examples**:

```bash
<JohnDoe@SYS:~> echo "Hello World" | wc
1 2 12
<JohnDoe@SYS:~> cat file.txt | grep "pattern" | wc -l
```

### Combined Redirection and Pipes

Supports I/O redirection combined with pipes.

**Examples**:

```bash
<JohnDoe@SYS:~> cat < input.txt | wc | cat > output.txt
<JohnDoe@SYS:~> echo "data" | sort > sorted.txt
```

## Signal Handling

### Keyboard Signals

- **Ctrl+C (SIGINT)**: Interrupts foreground process
- **Ctrl+Z (SIGTSTP)**: Stops foreground process and moves to background
- **Ctrl+D**: Exits shell gracefully

### Signal Handlers

- **SIGCHLD**: Handles background process completion
- **SIGINT**: Manages foreground process interruption
- **SIGTSTP**: Handles process suspension

## Networking Features

### iman - Internet Manual Pages

Fetches man pages from the internet using HTTP requests.

**Syntax**: `iman <command_name>`

**Features**:

- **HTTP Requests**: Connects to man.he.net
- **HTML Parsing**: Removes headers from response
- **Error Handling**: Handles missing man pages gracefully
- **Single Argument**: Ignores additional arguments

**Examples**:

```bash
<JohnDoe@SYS:~> iman sleep
[displays sleep man page]
<JohnDoe@SYS:~> iman invalid_command
[shows error message]
```

### neonate - Process Monitoring

Monitors the most recently created process on the system.

**Syntax**: `neonate -n <time_interval>`

**Features**:

- **Real-time Monitoring**: Continuously checks for new processes
- **Configurable Interval**: Set monitoring frequency
- **Interactive Exit**: Press 'x' to stop monitoring
- **Default Interval**: 5 seconds if no argument provided

**Examples**:

```bash
<JohnDoe@SYS:~> neonate -n 4
11810
11811
11812
[press 'x' to exit]
```

## Configuration

### .myshrc - Shell Configuration

Custom shell configuration file for aliases and functions.

**Location**: `~/.myshrc`

**Alias Syntax**:

```
alias_name = command
```

**Function Syntax**:

```
function_name()
{
    command1
    command2
}
```

**Built-in Functions**:

- `mk_hop()`: Create directory and change to it
- `hop_seek()`: Change to directory and search for matching files

**Example Configuration**:

```
reveall = reveal -l
alias home = hop ~

mk_hop()
{
    mkdir "$1"
    hop "$1"
}

hop_seek()
{
    hop "$1"
    seek "$1"
}
```

## Project Structure

### Source Files

- **main.c**: Main shell loop and signal handling
- **hop.c/h**: Directory navigation implementation
- **reveal.c/h**: File listing and directory operations
- **seek.c/h**: File search functionality
- **log.c/h**: Command history management
- **proclore.c/h**: Process information display
- **fgbg.c/h**: Foreground/background process control
- **ping.c/h**: Signal sending to processes
- **iman.c/h**: Internet man page fetching
- **neo.c/h**: Process monitoring
- **pio.c/h**: I/O redirection and pipes
- **background.c/h**: Background process management
- **bgprocess.c/h**: Background process tracking
- **queue.c/h**: Queue data structure for process management

### Header Files

- **IO.h**: I/O redirection constants and utilities
- **main.h**: Global variables and shared definitions

### Build Files

- **Makefile**: Compilation configuration
- **a.out**: Executable shell program

## Technical Details

### Compilation

- **Compiler**: GCC
- **Flags**: `-w -g` (warnings disabled, debug symbols)
- **Libraries**: `-lm` (math library)
- **Output**: `a.out` executable

### System Requirements

- **OS**: Linux/Unix (WSL for Windows)
- **Architecture**: x86/ARM compatible
- **Memory**: Minimal requirements
- **Dependencies**: Standard C libraries only

### Error Handling

- **Graceful Degradation**: Continues operation on command errors
- **User Feedback**: Clear error messages with color coding
- **Process Recovery**: Handles process failures without shell crash
- **File System**: Validates file operations and permissions

## Assumptions and Limitations

### Input Handling

- **Maximum Input Size**: 1100 characters
- **Whitespace**: Handles extra spaces and tabs automatically
- **Special Characters**: `<`, `>`, `>>`, `&`, `|`, `;` have special meanings
- **Path Assumptions**: No whitespace in file/directory names

### File System

- **WSL Compatibility**: Assumes WSL environment for Windows users
- **Permission Handling**: Basic permission validation
- **Hidden Files**: `.` prefix for hidden files
- **Path Resolution**: Supports relative and absolute paths

### Process Management

- **Background Process Limit**: Maximum 1024 tracked processes
- **Signal Handling**: Standard Unix signal behavior
- **Process States**: Running, Stopped, Zombie states
- **Terminal Control**: Proper foreground/background handoff

### Configuration

- **Hardcoded Paths**: Some paths are system-specific
- **Alias Support**: Single-word aliases only
- **Function Arguments**: Single argument functions
- **File Location**: `.myshrc` in home directory

### Network Features

- **HTTP Protocol**: Basic HTTP GET requests
- **Man Page Source**: man.he.net website
- **Error Handling**: Graceful handling of network failures
- **Content Parsing**: Basic HTML tag handling

### Performance

- **Memory Management**: Manual memory allocation/deallocation
- **Process Tracking**: Efficient background process monitoring
- **File Operations**: Optimized directory traversal
- **Command History**: FIFO queue with size limit

This C shell provides a comprehensive command-line environment with advanced features suitable for both learning and practical use. It demonstrates various Unix system programming concepts including process management, file operations, signal handling, and networking.
