#ifndef PING_H_
#define PING_H_

void ping(char * command);
void Ctrl_C(int sig);
void Ctrl_D(int sig);
void Ctrl_Z(int sig);
void ignore_sigstp();

#endif