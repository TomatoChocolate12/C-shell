#include "headers.h"

int redirect_input(char *filename) 
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("No such input file found!");
        return -1;
    }
    dup2(fd, STDIN_FILENO);
    return fd;
}

int redirect_create(char *filename) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open output file");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    return fd;
}

int redirect_append(char *filename) 
{
    int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        perror("open output file");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    return fd;
}

void iohandler(char *command, int *pipe_fd, char *prev, int flag)
{
    int start = 0;
    char delimiters[N];
    char *tokens[10];
    int check = tokenise_for_redirection(command, tokens, delimiters);
    char *output_file = NULL;
    char *append_file = NULL;
    char *input_file = NULL;
    int out_fd = -1, inp_fd = -1, app_fd = -1;
    int saved_stdout, saved_stdin;
    while (delimiters[start] != '\0') 
    {
        if (delimiters[start] == '>') 
        {
            saved_stdout = dup(STDOUT_FILENO);
            output_file = strdup(tokens[start + 1]);
            tokens[start + 1] = "\0";
        } 
        else if (delimiters[start] == '!') 
        {
            saved_stdout = dup(STDOUT_FILENO);
            append_file = strdup(tokens[start + 1]);
            tokens[start + 1] = "\0";
        } 
        else if (delimiters[start] == '<') 
        {
            saved_stdin = dup(STDIN_FILENO);
            input_file = strdup(tokens[start + 1]);
            tokens[start + 1] = "\0";
        }
        start++;
    }
    
    if(pipe_fd  != NULL)
    {
        dup2(*pipe_fd, STDOUT_FILENO);
    }
    if (output_file != NULL) 
    {
        out_fd = redirect_create(output_file);
    }
    if (append_file != NULL) 
    {
        app_fd = redirect_append(append_file);
    }
    if (input_file != NULL) 
    {
        inp_fd = redirect_input(input_file);
    }

    char *comm = strdup(tokens[0]);
    if (flag == 1) 
    {
        char *fore[12] = {NULL};
        fore[0] = comm;
        fprocess(fore, prev, NULL);
    } 
    else if (flag == 2) 
    {
        char *back[12] = {NULL};
        back[0] = comm;
        bprocess(back);
    }

    if (out_fd != -1 || app_fd != -1) 
    {
        dup2(saved_stdout, STDOUT_FILENO); // Restore stdout
        close(saved_stdout);
    }
    if (inp_fd != -1) 
    {
        dup2(saved_stdin, STDIN_FILENO); // Restore stdin
        close(saved_stdin);
    }
    if (out_fd != -1) 
    {
        close(out_fd);
        free(output_file);
    }
    if (app_fd != -1) 
    {
        close(app_fd);
        free(append_file);
    }
    if (inp_fd != -1) 
    {
        close(inp_fd);
        free(input_file);
    }
    free(comm);
}

void pipehandler(char **tokens, char *prev, int flag)
{
    int ind = 0;
    int curr_pipe[2];
    int in_fd = 0;
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);
    while (tokens[ind] != NULL) 
    {
        // printf("%s\n", tokens[ind]);
        // fprintf(stderr, "Debug: Processing command: %s\n", tokens[ind]);
        if (tokens[ind + 1] != NULL) 
        {
            if (pipe(curr_pipe) == -1) 
            {
                perror("Pipe creation failed");
                exit(EXIT_FAILURE);
            }
        }

        pid_t pid = fork();
        if (pid == -1) 
        {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) 
        {
            if (in_fd != 0) 
            {
                dup2(in_fd, STDIN_FILENO); // Redirect stdin
                close(in_fd);
            }
            if (tokens[ind + 1] != NULL) 
            {
                dup2(curr_pipe[1], STDOUT_FILENO); // Redirect stdout to pipe
                close(curr_pipe[1]);
            }
            close(curr_pipe[0]);

            iohandler(tokens[ind], NULL, prev, flag);
            exit(EXIT_SUCCESS);
        } 
        else 
        {
            // fprintf(stderr, "Debug: Parent process after forking %s\n", tokens[ind]);
            if (tokens[ind + 1] != NULL)
            {
                close(curr_pipe[1]);
            }
            if (in_fd != 0) 
            {
                close(in_fd);
            }
            if (tokens[ind + 1] != NULL)
            {
                in_fd = curr_pipe[0];
            }
            wait(NULL);
        }
        ind++;
    }
    dup2(saved_stdout, STDOUT_FILENO);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdout);
    close(saved_stdin);
    fflush(stdout);
}
