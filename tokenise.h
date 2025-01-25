#ifndef TOKENISE_H_
#define TOKENISE_H_

void tokenise(char * command, char ** foreground, char ** background);
void removeWhitespaces(char * str, char * ret);
int tokenise_for_redirection(char * command, char * ordered[], char delimiters[]);
int tokenise_for_piping(char * command, char * ordered[]);

#endif