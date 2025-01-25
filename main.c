#include "headers.h"

void main()
{
    signal(SIGINT, Ctrl_C);
    signal(SIGTSTP, Ctrl_Z);
    myshrc();
    char host[_SC_HOST_NAME_MAX + 1];
    int check_host;

    check_host = gethostname(host, sizeof(host));

    if(check_host != 0) // Checking error with hostname
    {
        perror("Error with retrieving hostname!");
        return;
    }

    struct passwd *pw;
    pw = getpwuid(getuid());

    if(pw == NULL) // Checking error with username
    {
        perror("Error with retrieving username!");
        return;
    }

    char dir[N];
    char command[N];

    if(getcwd(dir, sizeof(dir)) == NULL) // Checking error with cwd
    {
        perror("Error with retrieving current working directory!");
        return;
    }

    if (setenv("HOME", dir, 1) != 0) // Setting cwd as home
    {
        perror("Error with setting HOME environment variable");
        return;
    }

    char * home = getenv("HOME");
    int len_dir_name = strlen(home);
    char secs[N];
    char prev[N];
    secs[0] = '\0';
    char * forlog = (char*)malloc(sizeof(char) * N);
    char * check = (char*)malloc(sizeof(char) * N);

    int fd;
    if((fd = open("log.txt", O_RDONLY)) < 0) // Open log file to read
    {
        perror("Error opening log file to read");
        return;
    }

    char * buffer = (char *)malloc(sizeof(char) * 4096);
    if (read(fd, buffer, 4096) < 0) {
        perror("Error reading log file");
        free(buffer);
        close(fd);
        return;
    }
    close(fd);

    char * tok = strtok(buffer, "\n");
    while(tok != NULL)
    {
        free(check); // Free previous check string
        check = strdup(tok);
        tok = strtok(NULL, "\n");
    }
    free(buffer);

    while(1)
    {
        strcpy(prev, dir);
        if(getcwd(dir, sizeof(dir)) == NULL) // Checking error with cwd
        {
            perror("Error with retrieving current working directory!");
            continue;
        }

        printf(COLOR_BLUE "<%s@%s:~%s%s>" COLOR_RESET, pw->pw_name, host, dir + len_dir_name, secs);
        checkprocess();
        if (fgets(command, sizeof(command), stdin) == NULL) // Using fgets and checking for error
        {
            break;
            perror("fgets");
            continue;
        }
        
        command[strcspn(command, "\n")] = 0; // Remove trailing newline

        if(strcmp(command, check) != 0)
        {
            free(forlog); // Free previous forlog string
            forlog = strdup(command);
            int val = log_it(forlog);
            if(val == 1)
            {
                free(check); // Free previous check string
                check = strdup(command);
            }
        }

        if (strcmp(command, "exit") == 0) // Break if exit
        {
            break;
        }
        
        char * foreground[N] = {NULL};
        char * background[N] = {NULL};

        tokenise(command, foreground, background);

        secs[0] = '\0';
        fprocess(foreground, secs, prev);
        bprocess(background);

        // Free allocated memory for foreground and background tokens if necessary
    }

    free(forlog);
    free(check);
}
