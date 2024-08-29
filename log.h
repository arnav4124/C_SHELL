#pragma log
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
// #include "hop.h"
#include "main.h"
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
int check_log(char* str);

int line_counter();

void log_entry(char*str);

void log_call(char* args);
void log_print();
     