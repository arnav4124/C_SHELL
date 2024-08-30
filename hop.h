#ifndef HOP_H
#define HOP_H
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
// #include "log.h"
#define ll long long int
ll sl_cntr(char* str);
void  helper(char* args,char* cwd,char* cur_wd,int is_log);
void  hop(char* args,char* cwd,char* cur_wd,int is_log);
char* runner(char* str,char* cwd,char* cur_wd);
#endif
