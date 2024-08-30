#ifndef REVEAL_H
#define REVEAL_H
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
#include <sys/stat.h>
void reveal(char* args,char* cur,char* cwd);
int cmp(const void *a, const void *b);
void print_permissions(mode_t mode);
void print_file_info(const char *path, struct dirent *entry, int show_long);
void list_directory(const char *path, int show_all, int show_long,char* cmd);
void main_function(char* args,char* cwd,char* cur_wd,int is_log,char* cmd);
#endif