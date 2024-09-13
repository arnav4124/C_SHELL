#include "proclore.h"


int is_foreground(pid_t pid) {
    
    int pgid = getpgid(pid);
    if (pgid == -1) {
        perror("getpgid");
        return -1;  // Error
    }


    char tty_path[256];
    snprintf(tty_path, sizeof(tty_path), "/proc/%d/fd/0", pid);

    char tty_name[256];
    ssize_t len = readlink(tty_path, tty_name, sizeof(tty_name) - 1);
    if (len == -1) {
        perror("readlink");
        return -1;  
    }
    tty_name[len] = '\0';

    
    int fd = open(tty_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return -1;  
    }

   
    pid_t fg_pgid = tcgetpgrp(fd);
    close(fd);  

    if (fg_pgid == -1) {
        perror("tcgetpgrp");
        return -1;  
    }

   
    if (pgid == pid&& fg_pgid == pgid) {
        return 1;  
    } else {
        return 0;  
    }
}
char* checker(char* arg)
{
  static char buff[10260];
   int i=0;
   while(i<strlen(arg) && arg[i]!=':')
   {
       buff[i]=arg[i];
       i++;
   }
    buff[i]='\0';
    return buff;

}
void proclore(char* args)
{
    int proc_id;
    char copy[1045];
    strcpy(copy,args);
    if(strcmp(copy,"proclore")==0)
    {
       proc_id = getpid();
    }
    else{
    char dup[1024];
    strcpy(dup,args);
    char* tok=strtok(dup," ");
    tok=strtok(NULL," ");
    proc_id=atoi(tok);}
    char path[256];
    char buffer[1024];
    FILE *file;
    snprintf(path, sizeof(path), "/proc/%d/status", proc_id);
    file = fopen(path, "r");
    if (!file) {
        perror(RED"Could not open status file"RESET);
        return;
    }
    char state = ' ';
    int pgrp = 0;
    long vm_size = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        char* cpy=checker(buffer);
        if (strcmp(cpy,"State")==0) {
            sscanf(buffer, "State: %c", &state);
        } else if (strcmp(cpy,"Tgid")==0) {
            sscanf(buffer, "Tgid: %d", &pgrp);
        } else if (strcmp(cpy,"VmSize")==0) {
            sscanf(buffer, "VmSize: %ld", &vm_size);
        }
    }
    fclose(file);
    snprintf(path, sizeof(path), "/proc/%d/exe", proc_id);
    ssize_t len = readlink(path, buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
    } else {
        strcpy(buffer, "Unknown");
    }
    if(!is_foreground(proc_id)){
    
    printf("pid : %d\nprocess status : %c\nProcess Group : %d\nVirtual memory : %ld\nExecutable path : %s\n", proc_id,state,pgrp,vm_size,buffer);
    }
    else{
        printf("pid : %d\nprocess status : %c+\nProcess Group : %d\nVirtual memory : %ld\nExecutable path : %s\n", proc_id,state,pgrp,vm_size,buffer);
    }
            return;
}