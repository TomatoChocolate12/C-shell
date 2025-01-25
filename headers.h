#ifndef HEADERS_H_
#define HEADERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <dirent.h>
#include <grp.h>
#include <ftw.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <termios.h>
#include <sys/socket.h>
#include <netdb.h>

#define COLOR_RED "\033[31m"
#define COLOR_BLUE "\033[36m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RESET "\033[0m"

#define N 4096
#define MAX_ARGS 64

typedef struct bgpro{
    int pid;
    char *command;
    char status;
    int proid;
} bgpro;

typedef struct spawned{
    int pid;
    char * name;
    char * status;
}spawned;

typedef struct alias{
    char * command;
    char * alias;
}alias;

extern bgpro backs[N];
extern int totalpro;
extern pid_t fg_pid;
extern char * command;
extern spawned proc[N];
extern int procs;
extern pid_t main_pid;
extern alias alias_list[N];
extern int num_alias;

#include "tokenise.h"
#include "foreground.h"
#include "background.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "seek.h"
#include "redirection_and_piping.h"
#include "activities.h"
#include "ping.h"
#include "fgbg.h"
#include "neonate.h"
#include "iman.h"
#include "myshrc.h"

#endif