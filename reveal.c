#include "headers.h"

void print_permissions(mode_t mode)
{
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

int check_char(char * tok, char c)
{
    int len = strlen(tok);
    for (int i = 0; i < len; i++) 
    {
        if (tok[i] == c) 
        {
            return 1;
        }
    }
    return 0;
}


void list_files(char * directory, char ** listed)
{
    DIR * dr;
    struct dirent ** de;
    if ((dr = opendir(directory)) == NULL) 
    {
        perror("Directory could not be opened.");
        return;
    }

    int num_files = scandir(directory, &de, NULL, alphasort);
    if (num_files < 0) 
    {
        perror("Error with scanning the directory");
        closedir(dr);
        return;
    }

    for (int i = 0; i < num_files; i++)
        listed[i] = strdup(de[i]->d_name);
    
    closedir(dr);
}

void print_colored_name(struct stat filestat, char * filename)
{
    if (S_ISDIR(filestat.st_mode)) 
    {
        printf(COLOR_BLUE "%s" COLOR_RESET, filename);
    } 
    else if (filestat.st_mode & S_IXUSR)
    {
        printf(COLOR_GREEN "%s" COLOR_RESET, filename);
    } 
    else 
    {
        printf(COLOR_RED "%s" COLOR_RESET, filename);
    }
}

void reveal(char * command, char * prev)
{
    // struct dirent **de;
    int flag_l = 0;
    int flag_a = 0;
    char * tok;
    char * tokens[N] = {NULL};
    int idx = 0;
    
    tok = strtok(command, " \t");
    while (tok != NULL) 
    {
        if (tok[0] == '-' && (strlen(tok) > 1)) 
        {
            if (flag_l == 0)
                flag_l = check_char(tok, 'l');
            if (flag_a == 0)
                flag_a = check_char(tok, 'a');

            tok = strtok(NULL, " \t");
            continue;
        }
        tokens[idx++] = strdup(tok);
        tok = strtok(NULL, " \t");
    }

    if(idx == 1)
    {
        tokens[1] = ".";
        idx++;
    }

    for (int i = 1; i < idx; i++) 
    {
        if(strcmp(tokens[i], "-") == 0)
        {
            tokens[i] = strdup(prev);
            // printf("%s", tokens[i]);
        }
        char * listed[N] = {NULL};
        int ind = 0;
        list_files(tokens[i], listed);
        struct stat filestat;
        long total_blocks = 0;

        if (flag_l) 
        {
            while (listed[ind] != NULL) 
            {
                if (flag_a || listed[ind][0] != '.') 
                {
                    char filepath[N];
                    snprintf(filepath, sizeof(filepath), "%s/%s", tokens[i], listed[ind]);
                    if (stat(filepath, &filestat) < 0) 
                    {
                        perror("stat");
                        ind++;
                        continue;
                    }
                    total_blocks += filestat.st_blocks;
                }
                ind++;
            }
            printf("total %ld\n", total_blocks / 2);  // Divide by 2 because st_blocks is in 512-byte blocks
        }

        ind = 0;

        if (flag_l == 0 && flag_a == 0) 
        {
            // printf("yay\n");
            while (listed[ind] != NULL) 
            {
                if (listed[ind][0] != '.') 
                {
                    char filepath[N];
                    snprintf(filepath, sizeof(filepath), "%s/%s", tokens[i], listed[ind]);
                    if (stat(filepath, &filestat) < 0) 
                    {
                        perror("stat");
                        ind++;
                        continue;
                    }
                    print_colored_name(filestat, listed[ind]);
                    printf("\n");
                }
                ind++;
            }
        }
        if (flag_l == 0 && flag_a == 1) 
        {
            while (listed[ind] != NULL) 
            {
                char filepath[N];
                snprintf(filepath, sizeof(filepath), "%s/%s", tokens[i], listed[ind]);
                if (stat(filepath, &filestat) < 0) 
                {
                    perror("stat");
                    ind++;
                    continue;
                }
                print_colored_name(filestat, listed[ind]);
                printf("\n");
                ind++;
            }
        }
        if (flag_l == 1 && flag_a == 0) 
        {
            while (listed[ind] != NULL) 
            {
                if (listed[ind][0] != '.') 
                {
                    char filepath[N];
                    snprintf(filepath, sizeof(filepath), "%s/%s", tokens[i], listed[ind]);
                    if (stat(filepath, &filestat) < 0) 
                    {
                        perror("stat");
                        ind++;
                        continue;
                    }
                    print_permissions(filestat.st_mode);
                    printf(" %ld", filestat.st_nlink);
                    printf(" %s", getpwuid(filestat.st_uid)->pw_name);
                    printf(" %s", getgrgid(filestat.st_gid)->gr_name);
                    printf(" %ld", filestat.st_size);
                    
                    char timebuf[80];
                    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&filestat.st_mtime));
                    printf(" %s ", timebuf);

                    print_colored_name(filestat, listed[ind]);
                    printf("\n");
                }
                ind++;
            }
        }
        if (flag_l == 1 && flag_a == 1) 
        {
            while (listed[ind] != NULL) 
            {
                char filepath[N];
                snprintf(filepath, sizeof(filepath), "%s/%s", tokens[i], listed[ind]);
                if (stat(filepath, &filestat) < 0) 
                {
                    perror("stat");
                    ind++;
                    continue;
                }
                print_permissions(filestat.st_mode);
                printf(" %ld", filestat.st_nlink);
                printf(" %s", getpwuid(filestat.st_uid)->pw_name);
                printf(" %s", getgrgid(filestat.st_gid)->gr_name);
                printf(" %ld", filestat.st_size);

                char timebuf[80];
                strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&filestat.st_mtime));
                printf(" %s ", timebuf);

                print_colored_name(filestat, listed[ind]);
                printf("\n");
                ind++;
            }
        }
    }
    return;
}
