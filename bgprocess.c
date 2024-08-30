#include "bgprocess.h"
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
 extern fgproc fg[1024];
  extern  int cnt_fg;
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

    char name_proc[1000];
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
        
    
    char* token = strtok(str, " ");
        // printf("%c\n",str[strlen(str)-1]);
      
    
    char* args[100];
    int i=0;
    while(token!=NULL)
    {
        args[i++]=token;
        token = strtok(NULL, " ");
        
    }
    args[i]=NULL;
    
        if(execvp(args[0],args)==-1)
        {
            printf(RED"Error in executing command\n"RESET);
           return;
        }
       
    }
    else
    {
        // if(fl==1)
        // {
        //     printf("%d\n",ch);
        // }
        if(fl==0)
        {
           int wt= wait(NULL);
              if(wt<0)
              {
                printf(RED"Error in waiting\n"RESET);
                return;
              }
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
        }
        return;
    }
    }
}