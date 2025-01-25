#include "headers.h"

void hop_home()
{
    if(chdir(getenv("HOME")) != 0)
    perror("Could not change directory");
}

void hop(char * command, char * prev)
{
    char * tokens[N];
    char dir[N];
    char * tok = strtok(command, " \t");
    int idx = 0;
    while(tok != NULL)
    {
        tokens[idx++] = strdup(tok);
        tok = strtok(NULL, " \t");
    }
    for(int i = 1; i < idx; i++)
    {
        if(tokens[i][0] == '~' && strlen(tokens[i]) > 2) // for absolute paths
        {
            char * next =  tokens[i] + 2;
            hop_home();
            if(chdir(next) != 0)
            perror("Could not change directory");
        }
        else if(strcmp(tokens[i], "~") == 0) // to jump to home
        {
            hop_home();
        }
        else if(strcmp(tokens[i], "-") == 0) // to jump to prev directory
        {
            hop_home();
            if (chdir(prev) != 0)
            perror("Could not change directory");
        }
        else // to jump to relative path
        {
            if(chdir(tokens[i]) != 0)
            perror("Could not change directory");
        }
        if(getcwd(dir, sizeof(dir)) == NULL) //checking error with cwd
        {
            printf("Error with retrieving current working directory!");
        }
        printf("%s\n", dir);
    }
    if(idx == 1) // for hop without arguements
    {
        hop_home();
        if(getcwd(dir, sizeof(dir)) == NULL) //checking error with cwd
        {
            printf("Error with retrieving current working directory!");
        }
        printf("%s\n", dir);
    }
    return;
}