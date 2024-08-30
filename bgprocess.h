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
typedef struct fgproc{
    int pid;
    char name[1000];
    int time;
    }fgproc;

void sys_call(char* str);
#endif