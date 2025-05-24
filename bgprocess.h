#ifndef BGPROCESS_H
#define BGPROCESS_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>
#include <math.h> 
#include <sys/time.h> 
#include "background.h"
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
typedef struct fgproc{
    int pid;
    char name[1000];
    int time;
    }fgproc;

void sys_call(char* str);
void sigint_handler(int sig);
void sigtstp_handler(int sig);
#endif