#include "headers.h"

void seek_from_home(char ** found, char * tofind, char ** listed, int *num, char * dir)
{
    DIR * dr;
    struct dirent * entry;
    char path[N];

    if ((dr = opendir(dir)) == NULL) 
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dr)) != NULL) 
    {
        if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) 
        {
            perror("stat");
            continue;
        }

        if (strncmp(tofind, entry->d_name, strlen(tofind)) == 0)
        {
            found[*num] = strdup(path);
            (*num)++;
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            seek_from_home(found, tofind, listed, num, path);
        }
    }
    closedir(dr);
}

void seeking(char * command, char * prev)
{

    int flag_d = 0;
    int flag_f = 0;
    int flag_e = 0;
    char * tokens[100] = {NULL};
    int idx = 0;
    char * dup = strdup(command);
    char * tok = strtok(dup, " \t");
    while(tok !=  NULL)
    {
        if(tok[0] == '-' && strlen(tok) > 1)
        {
            if(strchr(tok, 'd'))
                flag_d = 1;
            if(strchr(tok, 'f'))
                flag_f = 1;
            if(strchr(tok, 'e'))
                flag_e = 1;
            tok = strtok(NULL, " \t");
            continue;
        }
        tokens[idx++] = strdup(tok);
        tok = strtok(NULL, " \t");
    }
    // if(strcmp(tokens[2], "-") == 0)
    // {
    //     tokens[2] = strdup(prev);
    //     // printf("%s", tokens[i]);
    // }
    char * listed[N] = {NULL};
    char * found[N] = {NULL};
    int num_found = 0;
    if(idx == 2)
    {
        tokens[2] = ".";
        seek_from_home(found, tokens[1], listed, &num_found, tokens[2]);
    }
    if(idx == 3)
    {
        seek_from_home(found, tokens[1], listed, &num_found, tokens[2]);
    }
    if(num_found == 0)
    {
        printf("No matches found\n");
    }
    if(flag_d == 0 && flag_f == 0)
    {
        struct stat filestat;
        char * filepath = (char*)malloc(sizeof(char)*N);
        for(int i = 0; i < num_found; i++)
        {
            
            sprintf(filepath, "%s/%s", tokens[2], found[i]);
            if (stat(filepath, &filestat) < 0) 
            {
                perror("stat");
                continue;
            }
            print_colored_name(filestat, found[i]);
            printf("\n");
        }
        
        if(flag_e == 1 && num_found == 1)
        {
            sprintf(filepath, "%s/%s", tokens[2], found[0]);
            if (stat(filepath, &filestat) < 0) 
            {
                perror("stat");
            }
            if(S_ISREG(filestat.st_mode))
            {
                int fd;
                char * buffer = (char*)malloc(sizeof(char)*N*100);
                if((fd = open(found[0], O_RDONLY, 0644)) < 0)
                {
                    perror("Missing permissions to open file.");
                }
                int bytes_read;
                if((bytes_read = read(fd, buffer, N*100)) < 0)
                    perror("Could not read file.");
                printf("%s\n", buffer);
                free(buffer);
                close(fd);
            }
            else if(S_ISDIR(filestat.st_mode))
            {
                char * inp = (char*)malloc(sizeof(char)*100);
                sprintf(inp, "hop %s", found[0]);
                // printf("%s", inp);
                hop(inp, prev);
                free(inp);
            }
        }
        free(filepath);
    }

    if(flag_d == 1 && flag_f == 1)
    {
        printf("Invalid Flags\n");
    }

    if(flag_d == 1)
    {
        struct stat filestat;
        int count = 0;
        char * dirs[100];
        for(int i = 0; i < num_found; i++)
        {
            char * filepath = (char*)malloc(sizeof(char)*N);
            sprintf(filepath, "%s/%s", tokens[2], found[i]);
            if (stat(filepath, &filestat) < 0) 
            {
                perror("stat");
                continue;
            }
            if(S_ISDIR(filestat.st_mode))
            {
                print_colored_name(filestat, found[i]);
                printf("\n");
                dirs[count++] = strdup(found[i]);
            }
            if(count == 0)
            {
                printf("No matches found\n");
            }
            free(filepath);
        }
        if(flag_e == 1 && count == 1)
        {
            char * inp = (char*)malloc(sizeof(char)*100);
            sprintf(inp, "hop %s", dirs[0]);
            // printf("%s", inp);
            hop(inp, prev);
            free(inp);
        }
    }

    if(flag_f == 1)
    {
        struct stat filestat;
        int count = 0;
        char * files[100];
        for(int i = 0; i < num_found; i++)
        {
            char * filepath = (char*)malloc(sizeof(char)*N);
            sprintf(filepath, "%s/%s", tokens[2], found[i]);
            if (stat(filepath, &filestat) < 0) 
            {
                perror("stat");
                continue;
            }
            if(S_ISREG(filestat.st_mode))
            {
                print_colored_name(filestat, found[i]);
                printf("\n");
                files[count++] = strdup(found[i]);
            }
            if(count == 0)
            {
                printf("No matches found\n");
            }
            free(filepath);
        }
        if(flag_e == 1 && count == 1)
        {
            int fd;
            char * buffer = (char*)malloc(sizeof(char)*N*100);
            if((fd = open(files[0], O_RDONLY, 0644)) < 0)
            {
                perror("Missing permissions to open file.");
            }
            int bytes_read;
            if((bytes_read = read(fd, buffer, N*100)) < 0)
                perror("Could not read file.");
            printf("%s\n", buffer);
            free(buffer);
            close(fd);
        }
    }

    free(dup);
    for(int i = 0; i < idx; i++)
        free(tokens[i]);
}