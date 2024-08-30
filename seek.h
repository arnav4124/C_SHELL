
#ifndef SEEK_H
#define SEEK_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pwd.h>
// #include <pwd.h>
#include <grp.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "queue.h"
#include <sys/stat.h>
#include "hop.h"
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"

void seek(char* args,char*cur,char*par,int is_log);
#endif