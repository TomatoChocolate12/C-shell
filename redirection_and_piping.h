#ifndef _REDIRECTION_AND_PIPING_H_
#define _REDIRECTION_AND_PIPING_H_

int redirect_input(char *filename);
int redirect_create(char *filename);
int redirect_append(char *filename);
void pipehandler(char ** tokens, char * prev, int flag);
void iohandler(char * command, int *pipe_fd, char * prev, int flag);

#endif