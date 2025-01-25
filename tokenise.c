#include "headers.h"

void removeWhitespaces(char * str, char * ret)
{
    char * tok =  strtok(str, " \t");
    int i = 0;
    while(tok != NULL)
    {
        int ind = 0;
        while(tok[ind] != '\0')
        {
            ret[i++] = tok[ind++];
        }
        ret[i++] = ' ';
        tok = strtok(NULL, " \t");
    }
    ret[--i] = '\0';
    return;
}

void tokenise(char * command, char * foreground[], char * background[])
{
    int fore = 0;
    int back = 0;
    char* tok1;
    char delims[N];
    char * tokens[N];
    int ind = 0;

    char * dup = strdup(command);
    tok1 = strtok(dup, "&;");

    while(tok1 != NULL)
    {
        char delim = command[tok1 - dup + strlen(tok1)];
        delims[ind] = delim;
        tokens[ind++] = tok1;
        tok1 = strtok(NULL, "&;");
    }

    delims[ind] = '\0';

    int start = 0;

    while(start < ind)
    {
        if(delims[start] == ';' || delims[start] == '\0')
        {
            foreground[fore++] = strdup(tokens[start]);
        }
        else if(delims[start] == '&')
        {
            background[back++] = strdup(tokens[start]);
        }
        start++;
    }

    foreground[fore] = NULL;
    background[back] = NULL;

    for(int i = 0; i < fore; i++)
    {
        char ret[strlen(foreground[i]) + 1];
        removeWhitespaces(foreground[i], ret);
        free(foreground[i]);
        foreground[i] = strdup(ret);
    }
    for(int i = 0; i < back; i++)
    {
        char ret[strlen(background[i]) + 1];
        removeWhitespaces(background[i], ret);
        free(background[i]);
        background[i] = strdup(ret);
    }

    free(dup); 
    return;
}

int tokenise_for_piping(char * command, char * ordered[])
{
    char * tok;
    char * dup = strdup(command);
    if(command[strlen(command) - 1] == '|')
    {
        fprintf(stderr, "ERROR: pipe with no outlet.\n");
        return 2;
    }
    tok = strtok(dup, "|");
    int ind = 0;
    while(tok != NULL)
    {
        ordered[ind++] = strdup(tok);
        // printf("%s\n", ordered[ind-1]);
        tok = strtok(NULL, "|");
    }
    // printf("%d\n", ind);
    for(int i = 0; i < ind; i++)
    {
        char ret[strlen(ordered[i]) + 1];
        removeWhitespaces(ordered[i], ret);
        ordered[i] = strdup(ret);
    }
    free(dup);
    if(ind == 1)
        return 0;
    else
        return 1;
}

int tokenise_for_redirection(char * command, char * ordered[], char delimiters[])
{
    char * tok;
    char delim;
    int ind = 0;

    char * dup = strdup(command);
    tok = strtok(dup, "><");
    while(tok != NULL)
    {
        delim = command[tok - dup + strlen(tok)];
        if (delim  == '>')
        {
            if (command[tok - dup + strlen(tok) + 1] == '>')
            {
                delim = '!';
            }
        }
        delimiters[ind] = delim;
        ordered[ind++] = strdup(tok);
        tok = strtok(NULL, "><");
    }
    delimiters[ind] = '\0';
    for(int i = 0; i < ind; i++)
    {
        char ret[strlen(ordered[i]) + 1];
        removeWhitespaces(ordered[i], ret);
        ordered[i] = strdup(ret);
    }
    if (ind == 1)
    {
        delimiters[0] = '\0';
        return 0;
    }
    else
        return 1;
}