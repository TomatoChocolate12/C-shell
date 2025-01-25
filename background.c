#include "headers.h"

bgpro backs[N];
int totalpro;

void removepro(int id)
{
    for (int i = 0; i < totalpro; ++i)
    {
        if (backs[i].pid == id)
        {
            for (int j = i + 1; j < totalpro; ++j)
            {
                backs[j - 1] = backs[j];
                backs[j - 1].proid--;
            }
            --totalpro;
            break;
        }
    }
}

void checkprocess()
{
    pid_t pid;
    int status;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        for (int i = 0; i < totalpro; ++i)
        {
            if (pid == backs[i].pid)
            {
                if (WIFEXITED(status))
                {
                    char *cmd = backs[i].command;
                    int exit_code = WEXITSTATUS(status);

                    if (exit_code == 0)
                        printf("%s exited normally (%d)\n", cmd, pid);
                    else
                        printf("%s exited abnormally (%d)\n", cmd, pid);

                    removepro(pid); // Remove the process entry once it's done
                }
                else if (WIFSIGNALED(status))
                {
                    printf("%s terminated by signal (%d)\n", backs[i].command, pid);
                    removepro(pid);
                }
                break;
            }
        }
    }
}

void bprocess(char **processes)
{
    int start = 0;

    while (processes[start] != NULL)
    {
        char *command = processes[start++];
        char *copy = strdup(command); // Tokenization copy
        char *ordered[N];
        char *dummy[N];
        char delimiters[N];

        // Tokenize the command
        int check = tokenise_for_piping(copy, ordered);
        int check2 = tokenise_for_redirection(copy, dummy, delimiters);

        if (check == 1)
        {
            pipehandler(ordered, NULL, 2);
            free(copy);
            continue;
        }
        else if (check2 == 1)
        {
            iohandler(command, NULL, NULL, 2);
            free(copy);
            continue;
        }
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("Fork unsuccessful");
        }
        else if (pid == 0)
        {
            setsid(); // Create a new session so it runs independently in background
            run_command(command);
            exit(0);
        }
        else
        {
            backs[totalpro].pid = pid;
            backs[totalpro].command = strdup(strtok(command, " \t\n"));
            backs[totalpro].proid = totalpro++;
            printf("Started background process: %s (%d)\n", backs[totalpro - 1].command, pid);
            fflush(stdout);
        }

        free(copy);
    }
    checkprocess();
}