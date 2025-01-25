#include "headers.h"

alias alias_list[N];
int num_alias = 0;

void myshrc() {
    char *filename = ".myshrc";
    char *line = NULL;
    ssize_t len = 0;
    FILE *fp = fopen(filename, "r");
    
    if (fp == NULL) {
        perror("Error opening the file");
        return;
    }

    while (getline(&line, &len, fp) != -1) {
        char *copy = strdup(line);
        if (!copy) {
            perror("Memory allocation failed");
            break;
        }

        char *tok = strtok(copy, " \t");
        if (tok && strcmp(tok, "alias") == 0) 
        {
            // printf("%s\n", tok);
            char * fuck = strdup(line + strlen(tok));
            tok = strtok(NULL, "=");
            // printf("%s\n", tok);
            if (tok) 
            {
                char ret[100];
                char * temp = strdup(tok);
                // printf("%s\n", temp);
                removeWhitespaces(temp, ret);
                // printf("1\n");
                alias_list[num_alias].alias = strdup(ret);
                // printf("1\n")
                // printf("%s\n", fuck + strlen(tok));;
                tok = strtok(fuck, "=");
                // printf("%s\n", tok);
                tok = strtok(NULL, "=");
                // printf("%s\n", tok);
                if(tok) 
                {
                    // printf("1\n");
                    char ret2[100];
                    removeWhitespaces(tok, ret2);
                    alias_list[num_alias].command = strdup(ret2);
                }
                num_alias++;
            }
        }
        free(copy);
    }
    if (line)
        free(line);
    fclose(fp);
}
