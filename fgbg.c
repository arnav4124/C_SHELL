#include "fgbg.h"
extern fgproc fg[1024];
  extern  int cnt_fg;
  extern int fg_pid;
    extern char fg_name[1000];
    char buf[1000]={0};
void fg_call(char* pid){
    int pid_int = atoi(pid);
   char* name = search_process(pid_int);
   if(name){
    // printf("My name is %s\n",name);
    fg_pid = pid_int;
    strcpy(fg_name,name);
    strcpy(buf,name);
    // printf("fg_name is %s\n",fg_name);
    remove_process(pid_int);}
    
    int pgid = getpgid(pid_int);
    int shellpgid = getpgid(0);
     struct timeval start, end;
        double elapsed_time;

        gettimeofday(&start, NULL);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        if (tcsetpgrp(0, pgid) == -1){
        // printf(RED "Error moving process to foreground.\n"RESET);
        // switch(errno){
        //     case EACCES:
        //         printf(RED "Permission denied\n"RESET);
        //         break;
        //     case EINVAL:
        //         printf(RED "Invalid argument\n"RESET);
        //         break;
        //     case EPERM:
        //         printf(RED "Operation not permitted\n"RESET);
        //         break;
        //     case ESRCH:
        //         printf(RED "No such process found\n"RESET);
        //         break;
        //     default:
        //         printf(RED "Error moving process to foreground.\n"RESET);
        //         break;
        // }
        }
        signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
   int ki= kill(pid_int,SIGCONT);
    if(ki==-1)
    {
        int err = errno;
        if(err==ESRCH)
        {
            printf("No such process found\n");
            return;
        }
        // printf("No such process found\n");
        if(err==EPERM)
        {
            printf("Permission denied\n");
            return;
        }
    }
    // printf("fg_pid is %d\n",pid_int);  
    int wt=waitpid(fg_pid,NULL,WUNTRACED);
    if(wt==-1)
    {
       printf("No such process found\n");

    }
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + 
                                (end.tv_usec - start.tv_usec) / 1000000.0;
    int rounded = (int)round(elapsed_time);
    // printf("cnt_fg--->%d\n",cnt_fg);
    if(rounded>=2){
        if(cnt_fg<1024){
            fg[cnt_fg].pid=pid_int;
          
         
            char ff[100]={0};
            if (name){
                int i = 0;
            for (; i < strlen(buf) && buf[i] != ' '; i++) {
                ff[i] = buf[i];
            }
            ff[i] = '\0';
            
               
            // printf("ff is %s\n",ff);  
             } 
            else printf("name not found\n");
            strcpy(fg[cnt_fg].name,ff);
            fg[cnt_fg].time=rounded;
            cnt_fg++;
        }
       else{
           printf("Cannot add more processes\n");
       }
    }
    signal(SIGTTOU, SIG_IGN);
    if (tcsetpgrp(0, shellpgid) == -1)
        printf(RED "Error passing control back to shell.\n" RESET);

    // setting the signals to their default values
    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
     
     
     
    return;
}
void bg_call(char* pid){
    int pid_int = atoi(pid);
    int ki=kill(pid_int,SIGCONT);
    if(ki==-1)
    {
        int err = errno;
        if(err==ESRCH)
        {
            printf("No such process found\n");
        }
        // printf("No such process found\n");
        if(err==EPERM)
        {
            printf("Permission denied\n");
        }
        
    }
    update_state(pid_int,"Running");
    return;
}