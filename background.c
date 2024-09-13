
    #include "background.h"
    extern int stdin_global;
    extern int stdout_global;
    // #include <asm-generic/signal-defs.h>
    #include <signal.h> // Add this line to include the necessary header file
    // #include <asm/signal.h>
    extern BackgroundProcess* head;
    
    #include "background.h"
#include <stdlib.h>
#include <string.h>

// Function to split the linked list into two halves
void split_list(BackgroundProcess* source, BackgroundProcess** frontRef, BackgroundProcess** backRef) {
    BackgroundProcess* fast;
    BackgroundProcess* slow;
    slow = source;
    fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            fast = fast->next;
            slow = slow->next;
        }
    }
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}

// Function to merge two sorted linked lists
BackgroundProcess* sorted_merge(BackgroundProcess* a, BackgroundProcess* b) {
    BackgroundProcess* result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (strcmp(a->command, b->command) <= 0) {
        result = a;
        result->next = sorted_merge(a->next, b);
    } else {
        result = b;
        result->next = sorted_merge(a, b->next);
    }
    return result;
}

void merge_sort(BackgroundProcess** headRef) {
    BackgroundProcess* head = *headRef;
    BackgroundProcess* a;
    BackgroundProcess* b;

  
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

   
    split_list(head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);

    
    *headRef = sorted_merge(a, b);
}

    void trim_trailing_whitespace(char *str) {
        if (str == NULL) return;

        int len = strlen(str);
        while (len > 0 && isspace((unsigned char)str[len - 1])) {
        //    printf("chsar--->%c",str[len--]) ;
        }
        str[len] = '\0';
    }
    void add_process(pid_t pid, const char* command,char* state) {
        BackgroundProcess* new_process = (BackgroundProcess*)malloc(sizeof(BackgroundProcess));
        new_process->pid = pid;
        strcpy(new_process->command, command);
        strcpy(new_process->state, state);
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
    char* search_process(pid_t pid) {
        BackgroundProcess* prev = NULL;
        BackgroundProcess* curr = head;

        while (curr != NULL) {
            if (curr->pid == pid) {
                return curr->command;
            }
            prev = curr;
            curr = curr->next;
        }
        return NULL;
    }
    void update_state(pid_t pid,char* state){
        BackgroundProcess* prev = NULL;
        BackgroundProcess* curr = head;

        while (curr != NULL) {
            if (curr->pid == pid) {
                strcpy(curr->state,state);
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    void kill_all(){
        BackgroundProcess* prev = NULL;
        BackgroundProcess* curr = head;

        while (curr != NULL) {
            kill(curr->pid,SIGKILL);
            prev = curr;
            curr = curr->next;
        }
    }
    void print_process(){
        BackgroundProcess* prev = NULL;
        BackgroundProcess* curr = head;
        if(curr==NULL){
            dprintf(stdout_global,RED"No background processes\n"RESET);
        }
        while (curr != NULL) {
            printf("PID: %d\nCommand: %s\nState: %s\n",curr->pid, curr->command, curr->state);
            prev = curr;
            curr = curr->next;
        }
    }   
    void sigtstp_ignore(int sig) {
        
    }
    // void tstp_handler(int sig) {
    //     pid_t pid = getpid();
    //     char* command = search_process(pid);
    //     if (command != NULL) {
    //         update_state(pid, "Stopped");
    //         dprintf(stdout_global,RED"Process %s (PID %d) stopped\n"RESET, command, pid);
    //     }
    // }
    // Signal handler for SIGCHLD
    void sigint_ignore(int sig) {
        
    }
    void sigchld_handler(int sig) {
        int status;
        pid_t pid;
        FILE *fp;
        fp=fopen("background.txt","a");
        int fl=0;
        while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0) {
            // char* cmd=search_process(pid);
            char* command = search_process(pid);
            
                
            
            if (command != NULL) {
                if(WIFSTOPPED(status)){
                    // printf("djkdbbjfb f4j3fb34f\n");
                     
                    update_state(pid,"Stopped");
                }
                else if(WIFCONTINUED(status)){
                    update_state(pid,"Running");
                }
                else{
                if(WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                dprintf(stdout_global,RED" %s exited abnormally (%d) with signal-Failed to execute %d\n"RED,command, pid, WEXITSTATUS(status));
                fwrite(command,1,strlen(command),fp);
            }         
            else if(WIFSIGNALED(status)){
                int termsig = WTERMSIG(status);
                if(termsig==SIGINT){
                    fl=1;
                }
                if(termsig!=SIGINT){
                dprintf(stdout_global,RED"Process %s (PID %d) was terminated by signal %d\n"RESET, command, pid, WTERMSIG(status));
                fwrite(command,1,strlen(command),fp);}
            }
            else if(WIFEXITED(status)) {
                dprintf(stdout_global,GREEN"Process %s (PID %d) exited normally with status %d\n"RESET, command, pid, WEXITSTATUS(status));
                fwrite(command,1,strlen(command),fp);
            }
            if(fl==0){
            // dprintf(stdout_global,"removing process\n");
            remove_process(pid);}
            }
            }
                // free(command);
            }
            fclose(fp);
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
            // printf("command--->%s\n",command);
            // Unblock SIGCHLD in child process
            //  if(command[0]=='\''){
            //     command++;
            //     command[strlen(command)-1]='\0';
            //  }
            // char* args[20] = {"/bin/sh", "-c", command, NULL};
            // struct sigaction sa4;
        // sa4.sa_handler = SIG_IGN;
        // sigemptyset(&sa4.sa_mask);
        // sa4.sa_flags = 0; // Use default flags
        // if (sigaction(SIGINT, &sa4, NULL) == -1) {
        //     perror("sigaction");
        //     exit(EXIT_FAILURE);
        // }
        // struct sigaction sa5;
        // sa5.sa_handler = SIG_IGN;
        // sigemptyset(&sa5.sa_mask);
        // sa5.sa_flags = 0; // Use default flags
        // if (sigaction(SIGTSTP, &sa5, NULL) == -1) {
        //     perror("sigaction");
        //     exit(EXIT_FAILURE);
        // }
             char * args[200];
      for(int i=0;i<200;i++)   {
          args[i]=(char*)malloc(41*sizeof(char));
      }
        char cpy[1000]={0};
        strcpy(cpy,command);
        int cnt=0;
        int i=0;
        while(i<strlen(cpy)){
            int j=0;
            if(cpy[i]=='\''){
                i++;
                while(cpy[i]!='\''){
                    args[cnt][j++]=cpy[i];
                    i++;
                }
                i++;
                args[cnt][j]='\0';
                // printf("%s\n",args[cnt]);
                cnt++;
                continue;
            }
            while(cpy[i]!=' ' && i<strlen(cpy)){
                args[cnt][j++]=cpy[i];
                i++;
            }
            args[cnt][j]='\0';
            // printf("%s\n",args[cnt]);
            cnt++;
            i++;
        }
        args[cnt]=NULL;
            // for(int j=0;j<i;j++)
            // {
            //     printf("%s\n",args[j]);
            // }
        // setsid();
        setpgrp();
        if( execvp(args[0], args)==-1){
            dprintf(stdout_global,RED"exec failed"RESET);
            exit(127);
            
        }
        } else {
            // Parent process
            int status;
            
            pid_t result = waitpid(pid, &status, WNOHANG);
            FILE *fp;
            fp=fopen("background.txt","a");
            if (result == 0) {
                // Process is still running, add to the list
                add_process(pid, command,"Running");
                int stdin_bak;
                int stdout_bak;
                //  dup2(stdin_bak,STDIN_FILENO);
                    // dup2(stdout_bak,STDOUT_FILENO);
                dprintf(stdout_global,"%d\n", pid); 
                // Print the PID of the background process
                //  close(stdin_bak);
                    // close(stdout_bak);
            } else {
                // Process finished before it could be added
                if (WIFEXITED(status)) {
                    if (WEXITSTATUS(status) == 127) {
                        dprintf(stdout_global,RED"Process %s (PID %d) failed to execute\n"RESET, command, pid);
                        fwrite(command,1,strlen(command),fp);
                    } 
                    else {
                        dprintf(stdout_global,GREEN"Process %s (PID %d) exited normally with status %d\n"RESET, command, pid, WEXITSTATUS(status));
                        fwrite(command,1,strlen(command),fp);
                    }
                } else if (WIFSIGNALED(status)) {
                    dprintf(stdout_global,RED"Process %s (PID %d) was terminated by signal %d\n"RESET, command, pid, WTERMSIG(status));
                    fwrite(command,1,strlen(command),fp);
                }
                

            // Unblock SIGCHLD
        }
        fclose(fp);
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
        // printf("background.c\n");
        // signal(SIGCHLD, sigchld_handler);  // Setup the signal handler for SIGCHLD
        
        //  printf("input--->%ssssss\n",input);
        int len = strlen(input);

    // Trim trailing spaces
    while (len > 0 && (input[len - 1] == ' '||input[len-1]=='\t')) {
        len--;
    }
    input[len] = '\0';
    if(strlen(input)==0){
        printf(RED"No input\n"RESET);
        return;
    }
        //    printf("after ----> %sguj\n",input);
        run_background_command(input);
    }

    // int main() {
    //     main_loop();
    //     return 0;
    // }
