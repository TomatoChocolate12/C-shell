#include "headers.h"

pid_t fg_pid = -1;
char * command;
pid_t main_pid = -1;

void run_command(char *command) 
{
    char *args[MAX_ARGS];
    char *token;
    int i = 0;

    // Tokenize the command by spaces
    token = strtok(command, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Last argument must be NULL for execvp

    // Execute the command using execvp
    if (execvp(args[0], args) == -1) {
        perror("execvp failed");
        fprintf(stderr, "%s is not a valid command\n", args[0]);
        exit(0);
    }
    return;
}

char* fprocess(char ** processes, char * ret, char * prev_dir)
{
    time_t start_time = time(NULL);
    int idx = 0;
    char * list[11] = {"hop", "reveal", "log", "proclore", "seek", "activities", "ping", "fg", "bg", "neonate", "iMan"};
    while (processes[idx] != NULL)
    {
        pid_t pid, pgid;
        pid = getpid();
        pgid = getpgid(pid);
        command = processes[idx++];
        // printf("%s\n", command);
        char * copy = strdup(command);
        char * copy2 = strdup(command);
        char * dup = strdup(command);
        char * tok =  strtok(dup, " \t");
        int flag = 0;
        char * ordered[N];
        char * dummy[N];
        char delimiters[N];
        int check = tokenise_for_piping(copy, ordered);
        int check2 = tokenise_for_redirection(copy2, dummy, delimiters);
        if (check == 1)
        {
            pipehandler(ordered, prev_dir, 1);
            continue;
        }
        else if(check2 == 1)
        {
            iohandler(command, NULL, prev_dir, 1);
            continue;
        }
        if(check == 2)
        {
            continue;
        }
        // printf("%d\n", num_alias);
        for (int i = 0; i < num_alias; i++) 
        {
            // printf("1\n");
            if (strcmp(alias_list[i].alias, tok) == 0) 
            {
                // printf("1\n");
                char *new_command = malloc(N);
                // printf("1\n");
                if (!new_command) 
                {
                    perror("Memory allocation failed");
                }
                strcpy(new_command, alias_list[i].command);
                char * temp = command + strlen(alias_list[i].alias);
                strcat(new_command, temp);
                free(command);
                // strcpy(tok, alias_list[i].alias);
                command = strdup(new_command);   
                tok = strtok(new_command, " \t");  
                // free(new_command);       
            }
        }

        for(int i = 0; i < 11; i++)
        {
            if(strcmp(tok, list[i]) == 0)
            {
                flag = 1;
                if (i == 0)
                {
                    hop(command, prev_dir);
                    // printf("%s", command);
                    // return ret;
                }
                else if (i == 1)
                {
                    reveal(command, prev_dir);
                    // return ret;
                }
                else if (i == 2)
                {
                    history(command, prev_dir);
                }
                else if (i == 3)
                {
                    proclore(command);
                    // return ret;
                }
                else if (i == 4)
                {
                    seeking(command, prev_dir);
                }
                else if(i == 5)
                {
                    activities();
                }
                else if(i == 6)
                {
                    ping(command);
                }
                else if(i == 7)
                {
                    fg(command);
                }
                else if(i == 8)
                {
                    bg(command);
                }
                else if(i == 9)
                {
                    neonate(command);
                }
                else if(i == 10)
                {
                    iman(command);
                }
            }
        }

        if(flag == 0 && check == 0 && check2 == 0)
        {
            // printf("%d\n", getpid());
            pid_t pid = fork();
            if (pid < 0)
            {
                perror("Error with forking");
                continue;
            }
            else if (pid == 0)
            {
                setpgid(0, 0);
                // printf("%d\n", getpgid(getpid()));
                run_command(command);
            }
            else
            {
                // printf("%d\n", getpgid(pid));
                // printf("%d\n", getpgid(getpid()));
                main_pid =getpid();
                fg_pid = pid;
                int status;
                waitpid(pid, &status, WUNTRACED);
                if (WIFEXITED(status) || WIFSIGNALED(status))
                {
                    fg_pid = -1;
                }
                time_t end_time = time(NULL);
                // printf("%ld\n", CLOCKS_PER_SEC);
                long int secs = (end_time - start_time);
                // printf("%ld\n", secs);
                if (secs > 2)
                {
                    sprintf(ret, "%s %s : %lds", ret, tok, secs);
                }
                else
                {
                    ret[0] = '\0';
                }
            }
        }
    }
    // printf("%s\n", ret);
    return ret;
}