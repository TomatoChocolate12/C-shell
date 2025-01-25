# Mini-Project Part A Report

## How to run:

Just type the following in the terminal for the directory containing all the files.

```
make
```

## Implementation of each specification

### Specification 1: Display Requirement [5]

This is done within the file `main.c` itself. The prompt is colored blue as directed. All arrays of strings, string lengths, etc. are initialised to N = 4096. This is assuming that the inputs do not exceed this limit.

### Specification 2: Input Requirements [5]

This is done with the help of the `strtok()` function as suggested. `tokenise.c` handles this. Both '&' and ';' are handled, with the assumption that the number of tokens (i.e., discrete commands per line) does not exceed N = 4096.

### Specification 3: hop [5]

This is implemented by the `hop.c` file. Assumption is made that the number of files per line does not exceed N = 4096;

### Specification 4: reveal [8]

This is implemented by the `reveal.c` file. Some assumptions are made like: the length of each filename does not exceed N = 4096 characters and the number of distinct files in a directory is not more than N = 4096. The total blocks printed with -l flag is changed to match the output recieved with `ls`.

### Specification 5: log [8]

This is implemented by the `log.c` file. All the logged commands are stored in the text file called `log.txt`. Some general assumptions made are: the number of characters in a filename does not cross 4096. The number of words in the log file is always under 10*N = 40960. Also, assumption is made that simply pressing the `enter` key is a command as it starts a new prompt. So, everytime the user just presses `enter` on their keyboard without typing a command, it accounts for a blank line, in the log. Also, the errorneous commands are also getting logged.

### Specification 6: System Commands [12]

This is implemented by the `foreground.c` and `background.c` files respectively. All the different commands that were implemented by me and were not system commands, are also foreground commands, but not background commands. If two foreground process take an excess of 5 seconds to complete, the next prompt will contain both the process names and the times they took. Erroneous background process commands print PID before exiting abnormally.

### Specification 7: proclore [5]
This is implemented by the `proclore.c` file. Some assumptions made are that: the number of characters in file name and the number of characters in the executable does not exceed N = 4096. The executable filename that is printed is printed with its absolute path, not relative path. Virtual memory is printed in kB.

### Specification 8: seek [8]
This is implemented by the `seek.c` file. Some assumptions that were made are: the filename has lesser than N = 4096 characters, the files whose content needs to be printed onto the terminal have lesser than N*100 characters, the output contains all files/directories which have the arguement as a prefix in their filename. Another assumption made is that the '-' cannot be a working directory attribute like it is for `hop` and `reveal`.

### Specification 9: myshrc[2 + 2]
Here the .myshrc file is parsed with a file handler to do the task. Assumption made is: the alias commands in .myshrc start with "alias".

### Specification 10: I/O Redirection [10]
Here one major assumption made was that there will be only one output redirection or input redirection per command. The program does not raise an error if output file does not exist. Assumption made that the command is read from left to right.

### Specification 11 : Pipes [12]
This function was implemented by using the pipes() syscall. Using a `&` anywhere makes the entire pipe a background process. Also it is assumed that commands where the `&` is in between a pipe command does not exist, i.e., commands like this: echo "lol" & | cat aren't going to work with my system. -this is how bash works as well. The number of commands within a pipe is at max 4096. Something like a 'hop .. | wc' will not change directories.

### Specification 12 : Redirection along with pipes [8]
No special assumptions here.

### Specification 13 : activities [5]
Assumed that the processes spawned by my shell are just processes whose ppid is my shell's pid. It is also assumed that the status code of 'T' means Stopped. Everything else means 'Running'. The number of activities stored at max is 4096.

### Specification 14 : Signals [7]
No special assumptions made. Implemented with the help of the `signal.h` library.

### Specification 15 : fg and bg [10]
Assumed that when `fg` is run, if any background process that comes to the foreground takes more than 5 seconds to run, it will not change the next input prompt.

### Specification 16 : Neonate [7] [BONUS]
After `x` is pressed, neonate will not print any more process ids. But the control is handed back to the terminal only after the shell sleeps for the time specified. If pid1 is greater than pid2, pid1 is assumed to be more recent than pid2.

### Specification 17 : iMan [4]
Not many assumptions made.

### Some general assumptions:
1. The '-' indicator for different directories points to the previous working directory from command before the current one. This means if I do `hop lol lmao -`, the previous working directory does not become lol.
2. If there are no previous directories, '-' is assumed to be the HOME directory.

### ChatGPT links:
https://chatgpt.com/share/66e6fc22-353c-800a-a197-e27dcc12079b
https://chatgpt.com/share/66e6fc34-2c9c-800a-ae5d-b937c2b05eee
https://chatgpt.com/share/66e6fc40-e4ec-800a-930d-a0741d3d37fd
https://chatgpt.com/share/66e6fc66-4bc4-800a-a961-1cea395338fc
https://chatgpt.com/share/66e6fc83-1d54-800a-a63b-d6942ac414f4
https://chatgpt.com/share/66e6fc9c-6ecc-800a-996b-829ca052b5bc
https://chatgpt.com/share/66e6fcaf-7fec-800a-98f0-63cfbafca7e7

I have not used GitHub Copilot for this assignment.