#ifndef NEO_H
#define NEO_H
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <termios.h>
#include <pthread.h>
#include <time.h>
#include "seek.h"
#include "main.h"
void neonate(char* args);
void disable_buffering();
void restore_buffering();
long long get_start_time(const char *pid);
char* get_most_recent_pid();
int check_exit_key();
void* key_monitor_thread(void *running_ptr);
#endif