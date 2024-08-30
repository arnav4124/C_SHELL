# ifndef PROCLORE_H
# define PROCLORE_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
// #include <string.h>
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"

char* checker(char* arg);

void proclore(char* args);
int is_foreground(pid_t pid);
# endif