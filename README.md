# Simple Command Line Interpreter

**University:** University of Belgrade, School of Electrical Engineering
**Course:** Object oriented programming 1  
**Year:** 2024/2025

## Overview

Simplified command-line interpreter that supports interactive and batch execution, redirection and pipes.

### Supported commands

| Command     | Description                                      |
|-------------|--------------------------------------------------|
| `echo`      | Passes input to output unchanged                 |
| `prompt`    | Changes the command prompt                       |
| `time`      | Prints current system time                       |
| `date`      | Prints current system date                       |
| `touch`     | Creates an empty file                            |
| `truncate`  | Clears the content of a file                     |
| `rm`        | Deletes a file                                   |
| `wc`        | Counts words (`-w`) or characters (`-c`)         |
| `tr`        | Replaces or removes a string                     |
| `head`      | Prints the first N lines (`-nN`)                 |
| `batch`     | Executes commands from input (batch mode)        |

### Additional features
- Input / output redirection (`<`, `>`)
- Pipes (`|`)
- Interactive mode and batch mode from file

