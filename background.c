
#include "background.h"
// #include <asm-generic/signal-defs.h>
#include <signal.h> // Add this line to include the necessary header file
// #include <asm/signal.h>

BackgroundProcess* head = NULL;

void trim_trailing_whitespace(char *str) {
    if (str == NULL) return;

    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
    //    printf("chsar--->%c",str[len--]) ;
    }
    str[len] = '\0';
}
void add_process(pid_t pid, const char* command) {
    BackgroundProcess* new_process = (BackgroundProcess*)malloc(sizeof(BackgroundProcess));
    new_process->pid = pid;
    strcpy(new_process->command, command);
    new_process->next = head;
    head = new_process;
}

// Function to remove a process from the list
char* remove_process(pid_t pid) {
    BackgroundProcess* prev = NULL;
    BackgroundProcess* curr = head;

    while (curr != NULL) {
        if (curr->pid == pid) {
            if (prev == NULL) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            char* command = strdup(curr->command);
            free(curr);
            return command;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

// Signal handler for SIGCHLD
void sigchld_handler(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char* command = remove_process(pid);
        if (command != NULL) {
            if (WIFEXITED(status)) {
                printf("Process %s (PID %d) exited normally with status %d\n", command, pid, WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Process %s (PID %d) was terminated by signal %d\n", command, pid, WTERMSIG(status));
            }
            free(command);
        }
    }
}

// Function to run a command in the background
void run_background_command(char* command) {
    trim_trailing_whitespace(command);
    // printf("command--->%s\n",command);
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);

    // Block SIGCHLD
    sigprocmask(SIG_BLOCK, &sigset, NULL);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);  // Unblock SIGCHLD before returning
        return;
    }

    if (pid == 0) {
        // Child process
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);  // Unblock SIGCHLD in child process
        char* args[] = {"/bin/sh", "-c", command, NULL};
       if( execvp(args[0], args)==-1){
        perror("exec failed");
        return;
        
       }
    } else {
        // Parent process
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);

        if (result == 0) {
            // Process is still running, add to the list
            add_process(pid, command);
            printf("%d\n", pid);  // Print the PID of the background process
        } else {
            // Process finished before it could be added
            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == 127) {
                    printf(RED"Process %s (PID %d) failed to execute\n"RESET, command, pid);
                } 
                else {
                    printf(GREEN"Process %s (PID %d) exited normally with status %d\n"RESET, command, pid, WEXITSTATUS(status));
                }
            } else if (WIFSIGNALED(status)) {
                printf(RED"Process %s (PID %d) was terminated by signal %d\n"RESET, command, pid, WTERMSIG(status));
            }
        }

        // Unblock SIGCHLD
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    }
}

// Function to process user input
void process_input(char* input) {
    // Check if the input command ends with "&"
    if (strstr(input, "&")) {
        // Remove '&' from the command and run it in the background
        char* command = strtok(input, "&");
        run_background_command(command);
    } else {
        // Run command in the foreground
        system(input);
    }
}

// Main loop of the shell
void main_loop(char* input) {
    // char input[1024];
    signal(SIGCHLD, sigchld_handler);  // Setup the signal handler for SIGCHLD
     
    //  printf("input--->%ssssss\n",input);
      int len = strlen(input);

// Trim trailing spaces
while (len > 0 && input[len - 1] == ' ') {
    len--;
}
input[len] = '\0';
    //    printf("after ----> %sguj\n",input);
    run_background_command(input);
}

// int main() {
//     main_loop();
//     return 0;
// }
