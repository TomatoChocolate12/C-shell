#include "headers.h"

void process_details(pid_t pid)
{
    char *filename = (char*)malloc(sizeof(char) * N);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char status[100] = {0};  // Initialize to ensure null-termination
    char *tg_id;
    pid_t pgid;

    if (pid == 0)
    {
        pid = getpid();
    }
    pgid = getpgid(pid);

    printf("PID: %d\n", pid);
    printf("PGID: %d\n", pgid);

    sprintf(filename, "/proc/%d/status", pid);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Process Status file open error");
        free(filename);
        return;
    }

    int line_number = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (line_number == 2)
        {
            char *state = strchr(line, '\t');
            if (state != NULL)
            {
                state = strtok(state, " \t\n");
                strncpy(status, state, sizeof(status) - 1);
                status[sizeof(status) - 1] = '\0';
            }
        }
        else if (line_number == 3)
        {
            char *tgid_str = strchr(line, '\t');
            if (tgid_str != NULL)
            {
                tgid_str = strtok(tgid_str, " \t\n");
                tg_id = tgid_str;
                int tgid = atoi(tg_id);
                if (tgid == pgid) // Foreground process
                {
                    strncat(status, "+", sizeof(status) - strlen(status) - 1);
                }
                printf("TGID: %s\n", tg_id);
                printf("Status: %s\n", status);
            }
        }
        else if (line_number == 17)
        {
            char *virt_mem = strchr(line, '\t');
            if (virt_mem != NULL)
            {
                virt_mem = strtok(virt_mem, " \t\n");
                printf("Virtual Memory: %s kB\n", virt_mem);
            }
        }
        line_number++;
    }

    free(line);
    fclose(fp);

    sprintf(filename, "/proc/%d/exe", pid);

    char exec[N];
    int index;
    if((index = readlink(filename, exec, N)) < 0)
        perror("Executable doesnt exist");
    exec[index] = '\0';
    printf("Executable path: %s\n", exec);

    free(filename);
    return;
}


void proclore(char* command)
{
    char *dup = strdup(command);
    char *tokens[3];
    char *tok = strtok(dup, " \t");
    int idx = 0;

    while (tok != NULL && idx < 2)
    {
        tokens[idx++] = strdup(tok);
        tok = strtok(NULL, " \t");
    }
    tokens[idx] = NULL;

    if (idx == 1)
    {
        process_details(0);
    }
    else if (idx == 2)
    {
        process_details(atoi(tokens[1]));
    }

    free(dup);  // Free the duplicated string
    return;
}
