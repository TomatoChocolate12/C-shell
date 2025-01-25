#include "headers.h"

int count_lines(char * buffer, int bytes_read)
{
    int count = 0;
    for(int i = 0; i < bytes_read; i++)
    {
        if(buffer[i] == '\n')
            count++;
    }
    return count;
}

int log_it(char *command)
{
    char *check = strdup(command);
    char *tok = strtok(check, "&;");
    char * filename = (char*)malloc(sizeof(char)*N);
    char * dir = getenv("HOME");
    sprintf(filename, "%s/log.txt", dir);
    while (tok != NULL)
    {
        char str[4] = {0};
        strncpy(str, tok, 3);
        str[3] = '\0';
        if (strcmp(str, "log") == 0)
        {
            free(check);
            return 0;
        }
        tok = strtok(NULL, "&;");
    }
    int fd;
    if ((fd = open(filename, O_APPEND | O_WRONLY | O_CREAT, 0644)) < 0)
    {
        perror("Error opening log file to log commands");
        free(check);
        return 0;
    }
    if (write(fd, command, strlen(command)) < 0 || write(fd, "\n", 1) < 0)
    {
        perror("Error writing to log file");
    }
    close(fd);
    free(check);
    return 1;
}

void print_log()
{
    char * filename = (char*)malloc(sizeof(char)*N);
    char * dir = getenv("HOME");
    sprintf(filename, "%s/log.txt", dir);
    int fd;
    if((fd = open(filename, O_RDONLY, 0644)) < 0)
    {
        perror("Error opening log file to read");
    }
    char buffer[N*10];
    read(fd, buffer, N*10);
    printf("%s", buffer);
    buffer[0] = '\0';
    close(fd);
}

void purge()
{
    char * filename = (char*)malloc(sizeof(char)*N);
    char * dir = getenv("HOME");
    sprintf(filename, "%s/log.txt", dir);
    int fd;
    if((fd = open(filename, O_WRONLY | O_CREAT, 0644)) < 0)
    {
        perror("Error opening log file to read");
    }
    if(ftruncate(fd, 0) < 0)
        perror("Could not purge the file");
    close(fd);
}

void execute(char *index, char * prev)
{
    char * filename = (char*)malloc(sizeof(char)*N);
    char * dir = getenv("HOME");
    sprintf(filename, "%s/log.txt", dir);
    char secs[4096];
    int id = strtol(index, (char **)NULL, 10);
    int fd;
    if((fd = open(filename, O_RDONLY, 0644)) < 0)
    {
        perror("Error opening log file to read");
    }
    char * buffer = (char *)malloc(sizeof(char) * N*10);
    int bytes_read = read(fd, buffer, N*10);
    int count = count_lines(buffer, bytes_read);
    close(fd);
    count = count - id;
    char * tok = strtok(buffer, "\n");
    while(tok != NULL && count != 0)
    {
        count--;
        // printf("%s\n", tok);
        tok = strtok(NULL, "\n");
    }
    char * foreground[4096] = {NULL};
    char * background[4096] = {NULL};
    tokenise(tok, foreground, background);
    secs[0] = '\0';
    fprocess(foreground, secs, prev);
    bprocess(background);
    if (strcmp(tok, "exit") == 0) // break if exit
    {
        exit(0);
    }
}

void history(char * command, char * prev)
{
    char * tokens[10];
    char * tok = strtok(command, " \t");
    int idx = 0;
    while (tok != NULL)
    {
        tokens[idx++] = tok;
        tok = strtok(NULL, " \t");
    }

    if(idx == 1)
    {
        print_log();
    }
    else if(strcmp(tokens[1], "purge") == 0)
    {
        purge();
    }
    else if(strcmp(tokens[1], "execute") == 0)
    {
        execute(tokens[2], prev);
    }
    return;
}