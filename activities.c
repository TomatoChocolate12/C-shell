#include "headers.h"

spawned proc[N];  // Process array
int procs;        // Number of processes

// Comparison function for sorting by PID
int compare_by_pid(const void *a, const void *b)
{
    spawned *proc_a = (spawned *)a;
    spawned *proc_b = (spawned *)b;
    return (proc_a->pid - proc_b->pid);
}

void activities()
{
    DIR *dir;
    ssize_t read;
    char *line = NULL;
    size_t len = 0;
    struct dirent *entry;

    // Initialize pid and pgid
    pid_t curr_pid = getpid(), curr_pgid = getpgid(curr_pid);

    if ((dir = opendir("/proc")) == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (isdigit(entry->d_name[0]))
        {
            char filename[N];
            char *check;
            char name[N] = "", status[30] = "Unknown";
            pid_t pid = -1, pgid = -1, ppid = -1;

            // Construct the file path to the /proc/[pid]/status file
            snprintf(filename, sizeof(filename), "/proc/%s/status", entry->d_name);

            FILE *fp = fopen(filename, "r");
            if (fp == NULL)
            {
                perror("Process Status file open error");
                continue;
            }

            int line_number = 0;
            while ((read = getline(&line, &len, fp)) != -1)
            {
                if (line_number == 0)
                {
                    char *exa = strchr(line, '\t');
                    if (exa != NULL)
                    {
                        exa = strtok(exa, " \t\n");
                        if (exa != NULL)
                        {
                            strncpy(name, exa, sizeof(name) - 1);
                            name[sizeof(name) - 1] = '\0';
                        }
                    }
                }
                else if (line_number == 2)
                {
                    char *state = strchr(line, '\t');
                    if (state != NULL)
                    {
                        state = strtok(state, " \t\n");
                        check = strdup(state);
                        if (state != NULL && state[0] == 'T')
                        {
                            snprintf(status, sizeof(status), "Stopped");
                        }
                        else
                        {
                            snprintf(status, sizeof(status), "Running");
                        }
                    }
                }
                else if (line_number == 3)
                {
                    char *pgid_str = strchr(line, '\t');
                    if (pgid_str != NULL)
                    {
                        pgid_str = strtok(pgid_str, " \t\n");
                        if (pgid_str != NULL)
                        {
                            pgid = atoi(pgid_str);
                        }
                    }
                }
                else if (line_number == 5)
                {
                    char *pid_str = strchr(line, '\t');
                    if (pid_str != NULL)
                    {
                        pid_str = strtok(pid_str, " \t\n");
                        if (pid_str != NULL)
                        {
                            pid = atoi(pid_str);
                        }
                    }
                }
                else if (line_number == 6)
                {
                    char *ppid_str = strchr(line, '\t');
                    if (ppid_str != NULL)
                    {
                        ppid_str = strtok(ppid_str, " \t\n");
                        if (ppid_str != NULL)
                        {
                            ppid = atoi(ppid_str);
                        }
                    }
                }
                if (pid != -1 && pgid != -1 && ppid != -1)
                {
                    break;
                }

                line_number++;
            }
            fclose(fp);

            if (ppid == curr_pid)
            {
                proc[procs].name = strdup(name);
                proc[procs].pid = pid;
                proc[procs].status = strdup(status); 
                procs++;
            }
        }
    }
    closedir(dir);
    if (line)
    {
        free(line);
    }

    qsort(proc, procs, sizeof(spawned), compare_by_pid);

    for (int i = 0; i < procs; i++)
    {
        printf("%d\t%s\t:%s\n", proc[i].pid, proc[i].name, proc[i].status);
        free(proc[i].name);   // Free memory allocated by strdup
        free(proc[i].status); // Free memory allocated by strdup
    }
    procs = 0;
    return;
}
