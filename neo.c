#include "neo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>
#include <sys/select.h>
extern fgproc fg[1024];
 extern   int cnt_fg;
struct termios old_tio;
extern 

void disable_buffering() {
    struct termios new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~ICANON;  // Disable canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Restore original terminal settings
void restore_buffering() {
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}


long long get_start_time(const char *pid) {
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);

    FILE *stat_file = fopen(path, "r");
    if (!stat_file) {
        return -1;  
    }

    long long start_time;
    char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stat_file) != NULL) {
        char *token = strtok(buffer, " ");  
        for (int i = 1; i < 22; ++i) {
            token = strtok(NULL, " ");  
        }
        start_time = atoll(token);
    }
    fclose(stat_file);
    return start_time;
}

// Get the PID of the most recently created process in /proc
char* get_most_recent_pid() {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("opendir");
        return NULL;
    }

    struct dirent *entry;
    long long latest_time = -1;
    static char recent_pid[16];

    while ((entry = readdir(proc_dir)) != NULL) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            long long start_time = get_start_time(entry->d_name);
            if (start_time > latest_time) {
                latest_time = start_time;
                strncpy(recent_pid, entry->d_name, sizeof(recent_pid) - 1);
                recent_pid[sizeof(recent_pid) - 1] = '\0';
            }
        }
    }

    closedir(proc_dir);
    return latest_time == -1 ? NULL : recent_pid;
}


int check_for_exit_key() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    struct timeval timeout = {0, 0};  

    int result = select(STDIN_FILENO + 1, &fds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        return 0;
    } else if (result > 0) {
        char ex;
        if (read(STDIN_FILENO, &ex, 1) == 1 && ex == 'x') {
            return 1;  // Exit if 'x' is pressed
        }
    }

    return 0;
}

void neonate(char* args) {
    int time_arg = atoi(args);
    if (time_arg <= 0) {
        printf(RED"Invalid time argument\n"RESET);
        return;
    }
    time_t s=time(NULL);
    int g=getpid();
    disable_buffering();  

    int running = 1;

    while (running) {
        char* cmd = get_most_recent_pid();
        time_t now = time(NULL);
        time_t end = now + time_arg;

        if (cmd == NULL) {
            printf(RED"No process running\n"RESET);
            restore_buffering();
            break;
        }
        
        printf("Most recent process: %s\n", cmd);
        fflush(stdout);  

       
        while (time(NULL) < end) {
            if (check_for_exit_key()) {  
                running = 0;
                restore_buffering();
                time_t e=time(NULL);
                int t_elapsed=e-s;
                printf("Time elapsed: %d seconds\n", t_elapsed);
                if(t_elapsed>2){
                   if(cnt_fg<1024){
                    fg[cnt_fg].pid=g;
                    strcpy(fg[cnt_fg].name,"neonate");
                    fg[cnt_fg].time=(int)round(t_elapsed);
                    cnt_fg++;
                    }
                    else{
                        printf(RED"Maximum number of processes reached\n"RESET);
                    }
                }
                
                return;
            }
        }
    }

    restore_buffering();
}
