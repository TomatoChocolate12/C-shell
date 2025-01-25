#include "headers.h"

void fg(char * command)
{
    char * tok = strtok(command, " \t\n");
    tok = strtok(NULL, " \t\n");
    pid_t pid = atoi(tok);
    fg_pid = pid;
    kill(pid, SIGCONT);
    int status;
    waitpid(fg_pid, &status, WUNTRACED);
    fg_pid = -1;
    return;
}

void bg(char * command)
{
    char * tok = strtok(command, " \t\n");
    tok = strtok(NULL, " \t\n");
    pid_t pid = atoi(tok);
    kill(pid, SIGCONT);
    return;
}