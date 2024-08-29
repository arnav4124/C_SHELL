#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
// Structure to hold information about background processes
typedef struct BackgroundProcess {
    pid_t pid;
    char command[1024];
    struct BackgroundProcess* next;
} BackgroundProcess;

void add_process(pid_t pid, const char* command);
char* remove_process(pid_t pid);
void sigchld_handler(int sig);
void run_background_command(char* command);
void process_input(char* input);
void trim_trailing_whitespace(char *str);
void main_loop(char* input);