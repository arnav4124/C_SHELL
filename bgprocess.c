#include "bgprocess.h"

 extern fgproc fg[1024];
  extern  int cnt_fg;
  extern int  stdout_global;
  extern int  stdin_global;
  extern pid_t fg_pid;
    extern char fg_name[1000];
//   void sigint_handler(int sig) {
//     if (fg_pid != -1) {
//         // printf("\nInterrupting foreground process %d\n", fg_pid);
//         kill(fg_pid, SIGINT); // Send SIGINT to the foreground process

//     } else {
//         printf("\n");
//     }
// }
// void sigtstp_handler(int sig) {
//     // printf("gotcha\n");
//     // int stdin_ba
//     if (fg_pid != -1) {  
//         printf("\nStopping foreground process %d\n", fg_pid);
//         if(kill(fg_pid, SIGTSTP)==-1){
//             printf("Error in stopping process\n");
//         }
//         printf("4r4t5\n");
//         add_process(fg_pid, fg_name, "Stopped");
//         fg_pid = -1; 
//         return;
//     } else {
//         printf("errrrrrrr\n");
//         printf("\n");
//     }
// }
void sys_call(char* str)
{
    
    // printf("%s---> %lld\n",str,strlen(str));
    int fl=0;
    // printf("str----->%s\n",str);
    if(str[strlen(str)-1]=='&')
    {
        fl=1;
        str[strlen(str)-1]='\0';
       
    }
    strcpy(fg_name,str);
    char name_proc[1000]={0};
    for(int i=0;i<strlen(str);i++)
    {
        if(str[i]==' ')
        {
            break;
        }
        name_proc[i]=str[i];
    }
    
    name_proc[strlen(name_proc)]='\0';
    if(fl)
    {
        // printf("back\n");
         main_loop(str);
    }
    else{
        struct timeval start, end;
        double elapsed_time;
        gettimeofday(&start, NULL);
    int  ch=fork();
        
    
    if(ch<0)
    {
        printf(RED"Error in running process\n"RESET);
        return;
    }
    else if(ch==0)
    {
        
    
    // char* token = strtok(str, " ");
    //     // printf("%c\n",str[strlen(str)-1]);
      
    
    // char* args[100];
    // int i=0;
    // while(token!=NULL)
    // {
    //     char cpy[1000];
    //     strcpy(cpy,token);
    //     if(cpy[0]=='\''){
    //         cpy[strlen(cpy)-1]='\0';
    //         for(int j=0;j<strlen(cpy);j++)
    //         {
    //             cpy[j]=cpy[j+1];
    //         }
    //     }
    //     printf("%s\n",cpy);
    //     args[i++]=cpy;
    //     token = strtok(NULL, " ");
        
    // }
    // args[i]=NULL;
    //  if(str[0]=='\''){
    //         str++;
    //         str[strlen(str)-1]='\0';
    //      }
    //     char* args[] = {"/bin/sh", "-c", str, NULL};
    
    //     if(execvp(args[0],args)==-1)
    //     {
    //         printf(RED"Error in executing command\n"RESET);
    //        return;
    //     }
      char * args[200];
      for(int i=0;i<200;i++)   {
          args[i]=(char*)malloc(41*sizeof(char));
      }
        char cpy[1000]={0};
        strcpy(cpy,str);
    //     char * save54;

    //     char * token45 = strtok_r(cpy, " ", &save54);
    //     int i=0;
    //    while (token45 != NULL) {
    // char cpy2[1000] = {0};
    // strcpy(cpy2, token45);
    // if (cpy2[0] == '\'') {
    //     cpy2[strlen(cpy2) - 1] = '\0';
    //     for (int j = 0; j < strlen(cpy2); j++) {
    //         cpy2[j] = cpy2[j + 1];
    //     }
    // }
    // // Allocate memory for the argument and copy the content
    // args[i] = malloc(strlen(cpy2) + 1);
    // if (args[i] == NULL) {
    //     perror("malloc");
    //     exit(EXIT_FAILURE);
    // }
    // strcpy(args[i], cpy2);
    // i++;
    // token45= strtok_r(NULL, " ", &save54);
// }
       
        // args[i]=NULL;
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
    //      struct sigaction sa4;
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
        // for(int j=0;j<i;j++)
        // {
        //     printf("%s\n",args[j]);
        // }
        setsid();
        //  setpgrp();
    //    if(strcmp(args[0],"vim")!=0) setpgid(0,0);
       if( execvp(args[0], args)==-1){
           dprintf(stdout_global,RED"Error in executing command\n"RESET);
            exit(0);
       }
       
    }
    else
    {
        // if(fl==1)
        // {
        //     printf("%d\n",ch);
        // }
        

        fg_pid = ch;
        if(fl==0)
        {
           int status;
                  int wt=waitpid(ch, &status, WUNTRACED);  
            if(wt==-1 && errno!=EINTR)
            {
                printf(RED"Error in waiting\n"RESET);
                return;
            }
            //   if(wt<0)
            //   {
            //     printf(RED"Error in waiting\n"RESET);
            //     return;
            //   }
            // printf("dyegfbfkchrefn vkjr\n");
            gettimeofday(&end, NULL);
             elapsed_time = (end.tv_sec - start.tv_sec) + 
                               (end.tv_usec - start.tv_usec) / 1000000.0;
            int rounded = (int)round(elapsed_time);
            if(rounded>=2){
                  
                  if(cnt_fg<1024){
                     fg[cnt_fg].pid=ch;
                        strcpy(fg[cnt_fg].name,name_proc);
                        fg[cnt_fg].time=rounded;
                        cnt_fg++;
                  }
            }
            fg_pid = -1;
        return;
        }
    }
    }
}