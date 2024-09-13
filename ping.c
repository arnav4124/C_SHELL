#include "ping.h"

void ping(char* args) {

    char* save33;
    char* tok33 = strtok_r(args, " ", &save33);
    int i = 0;
    int pid;
    int sig;

    while(tok33 != NULL) {
        if(i == 0) {
            pid = atoi(tok33);  
        } else if(i == 1) {
            sig = atoi(tok33);  
        } else {
            printf("Error: Invalid number of arguments\n");
            return;
        }
        tok33 = strtok_r(NULL, " ", &save33);
        i++;  
    }

    if (kill(pid, sig) == -1) { 
        perror("Error sending signal");
        return;
    }

    printf("Signal %d sent to process %d\n", sig, pid);
}