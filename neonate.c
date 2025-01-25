#include "headers.h"

int get_recent_pid() 
{
    DIR *dir;
    struct dirent *entry;
    pid_t recent_pid = 0;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR) 
        {
            if (isdigit(entry->d_name[0])) 
            {
                pid_t pid = atoi(entry->d_name);
                if (pid > recent_pid) 
                {
                    recent_pid = pid;
                }
            }
        }
    }

    closedir(dir);
    return recent_pid;
}

int is_key_pressed() 
{
    struct termios old_term, new_term;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch == 'x') {
        return 1;
    }
    return 0;
}

void neonate(char * command)
{
    char * tokens[3];
    char * tok = strtok(command, " \t\n");
    int ind = 0;
    while(tok != NULL)
    {
        tokens[ind++] = strdup(tok);
        tok = strtok(NULL, " \t\n");
    }
    // printf("%s\n", tokens[2]);
    int time_interval = atoi(tokens[2]);
    if(time_interval < 0)
    {
        fprintf(stderr, "Invalid time arguement\n");
    }
    // printf("%d\n", time_interval);
    while(1)
    {
        pid_t pid = get_recent_pid();
        if(pid > 0)
        {
            printf("%d\n", pid);
        }
        sleep(time_interval);
        if(is_key_pressed())
        {
            break;
        }
    }
    return;
}