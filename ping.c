#include "headers.h"

void ping(char * command)
{
    char * tokens[3];
    char * tok = strtok(command, " \t\n");
    int ind = 0;
    while(tok != NULL)
    {
        tokens[ind++] = strdup(tok);
        tok = strtok(NULL, " \t\n");
    }
    int pid = atoi(tokens[1]);
    int sig = atoi(tokens[2]);
    sig = sig % 32;
    if(pid < 0)
    {
        fprintf(stderr, "Erroneous PID");
        return;
    }
    // printf("%d %d", pid, sig);
    if((kill(pid, sig)) == 0)
    {
        printf("Sent signal %d to process with PID %d\n", sig, pid);
    }
    else
    {
        perror("Error sending signal");
    }
    return;
}

void Ctrl_C(int sig)
{
    if(fg_pid != -1)
    {
        kill(fg_pid, SIGINT);
    }
    return;
}

void Ctrl_Z(int sig)
{
        if(fg_pid != 0){ //If a background process is running...

        //We will check if the process has exited.
        int status;
        pid_t exited_process = waitpid(fg_pid, &status, WNOHANG);

        if(exited_process < 0){
            fg_pid = 0;
        }
        else
        {
            kill(fg_pid, SIGTSTP);
            for(int i = 0; i < totalpro; i++)
            {
                if(backs[i].pid == 0){

                    // We need to add it to background_processes.
                    backs[i].pid = fg_pid;
                    strcpy(backs[i].command, command);
                    backs[i].proid = totalpro++;

                    //Lets switch back io ports to the main shell.
                    tcsetpgrp(STDIN_FILENO, getpgid(main_pid));
                    tcsetpgrp(STDOUT_FILENO, getpgid(main_pid));

                    printf("\nStopped the process with the name \"%s\"...\n", command);

                    fg_pid = 0;
                    break;
                }
            }
            printf("\n");
            return;
        }
    }
}