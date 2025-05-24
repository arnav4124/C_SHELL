#ifndef FGBG_H
#define FGBG_H
#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#include <netinet/in.h>
#include "background.h"
#include "bgprocess.h"
#include <sys/time.h>  
void fg_call(char* pid);
void bg_call(char* pid);
#endif