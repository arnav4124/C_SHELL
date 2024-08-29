#include "proclore.h"



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
    printf("pid : %d\nprocess status : %c\nProcess Group : %lld\nVirtual memory : %lld\nExecutable path : %s\n", proc_id,state,pgrp,vm_size,buffer);
        return;
}