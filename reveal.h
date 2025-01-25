#ifndef REVEAL_H_
#define REVEAL_H_

int check_char(char * tok, char c);
void list_files(char * directory, char ** listed);
void reveal(char * command, char * prev);
void print_permissions(mode_t mode);
void print_colored_name(struct stat filestat, char * filename);


#endif