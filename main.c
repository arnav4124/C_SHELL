
#include <stdio.h>
#include <stdlib.h>
// #define RESET "\x1B[0m"X

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pwd.h>
#include "hop.h"
#include "reveal.h"
#include "seek.h"
#include "bgprocess.h"
#include <string.h>
#include "log.h"
#include "proclore.h"
#include "fgbg.h"
#include "pio.h"
#include "iman.h"
char* qu[15];
  int l=0;
  int r=0;
  int cnt;
// #include <sys/wait.h>
char u_name[256];
pid_t fg_pid = -1;
  char fg_name[1000]={0};
 char h_name0[256];
  char cwd[1024];
   char cur_wd[1000];
   char prev_wd[1000];
   int is_log=0;
   fgproc fg[1024];
   int cnt_fg=0;
   int stdin_global;
    int stdout_global;
    BackgroundProcess* head = NULL;
      void sigint_handler(int sig) {
    if (fg_pid != -1) {
        // printf("\nInterrupting foreground process %d\n", fg_pid);
        kill(fg_pid, SIGINT); // Send SIGINT to the foreground process

    } else {
        printf("\n");
    }
}
void sigtstp_handler(int sig) {
    // printf("gotcha\n");
    // int stdin_ba
    if (fg_pid != -1) {  
        printf("\nStopping foreground process %d\n", fg_pid);
        if(kill(fg_pid, SIGSTOP)==-1){
            printf("Error in stopping process\n");
        }
        // printf("4r4t5\n");
        add_process(fg_pid, fg_name, "Stopped");
        fg_pid = -1; 
        return;
    } else {
        // printf("errrrrrrr\n");
        printf("\n");
    }
}
// void logentr(char* str){
//    FILE *file = fopen("log.txt", "a+");
//     if(file==NULL)
//     {
//          printf("Error in opening file\n");
//          return;
//     }
//     char line[1000]=fgets(line, sizeof(line), file);
//    if(strcmp(str,"log")!=0&&strcmp(line,"str")) fprintf(file, "%s\n", str);
//    cnt++;
//     fclose(file);
   

// }
// void log_call(){
//     FILE *file = fopen("log.txt", "r");
//     if(file==NULL)
//     {
//          printf("Error in opening file\n");
//          return;
//     }
//     char line[1000];
//     int i=0;
//     while(fgets(line, sizeof(line), file))
//     {
//        if(cnt-i<=15) printf("%s", line);
//        i++;
//     }
//     fclose(file);
// }
void tok(char* str)
{
    char copy[1024]={0};
    strcpy(copy,str);
    char* save;
    char* token=strtok_r(copy,";",&save);
  
   
        // strcpy(args, cwd);
    
        while(token!=NULL)
        {
        //    char command[1000];
            while (*token == ' '||*token=='\t') token++;
            char* end = token + strlen(token) - 1;
            while (end > token && (*end == ' '||*end=='\t')) end--;
            *(end + 1) = '\0';
           int done=0;
        //    char args[1000];
           char b[1000];
           char cpy[1024];
           strcpy(cpy,token);
           char cpy_io[1024];
              strcpy(cpy_io,token);
            if(strstr(cpy_io,"|")){
                  execute_pipeline(cpy_io,cwd,cur_wd);
                  token=strtok_r(NULL,";",&save);
                  continue;
            }
            else{
            int is_IO=execute_command(cpy_io,cur_wd,cwd);
            if (is_IO) {
                 token=strtok_r(NULL,";",&save);
                 continue;}
                            getcwd(b,sizeof(b));}
       
            
       
            token=strtok_r(NULL,";",&save);
        }
}
int main()
{
      stdin_global=dup(STDIN_FILENO);
    stdout_global=dup(STDOUT_FILENO);
    struct sigaction sa;
    sa.sa_handler = &sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    struct sigaction sa2;
    sa2.sa_handler = &sigtstp_handler;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = SA_RESTART;
    if(sigaction(SIGTSTP, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    struct sigaction sa3;
    sa3.sa_handler = &sigint_handler;
    sigemptyset(&sa3.sa_mask);
    sa3.sa_flags = SA_RESTART; // Use default flags
    if (sigaction(SIGINT, &sa3, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

     

//   int ch2=getlogin_r(u_name, sizeof(u_name));
//   {
//         if(ch2==-1)
//         {
//             printf("Error in getting username\n");
//             exit(1);
//         }
//   }
    uid_t uid = getuid();            // Get the user ID
    struct passwd *pw = getpwuid(uid); // Get the password structure

    if (pw) {
        // printf("Hello, %s!\n", pw->pw_name); // Print the username
    } else {
        perror(RED"Error in getting username"RESET);
        return 1;
    }
//   printf("Hello, %s!\n", u_name);
   
   int ch= gethostname(h_name0, sizeof(h_name0));
   if(ch==-1)
   {
         printf(RED"Error in getting hostname\n"RESET);
         exit(1);
   }
    // printf("Hello, %s! You are on %s\n", u_name, h_name0);
   
    getcwd(cwd, sizeof(cwd));
    // printf("%s\n", cwd);
    if(cwd==NULL)
    {
         printf(RED"Error in getting current working directory\n"RESET);
        exit(1);
    }
    strcpy(prev_wd, cwd);
    strcpy(cur_wd, "~\0");
    // printf("%d\n", (int)strlen(cur_wd));

    while(1)
    {
       
  
     
        char fg_prefix[1000];
        int is_prefix=0;
        if(cnt_fg!=0)
        {
            //  for(int i=0;i<cnt_fg;i++)
            //  {
            //     if(fg[i].time>2){
            //         strcat(fg_prefix, fg[i].name);
            //         strcat(fg_prefix, ": ");
            //         strcat(fg_prefix, fg[i].time);
            //         strcat(fg_prefix, " ");
            //     }
                
            //  }
                is_prefix=1;
            //  cnt_fg=0;
        }
        if(is_prefix==1){
              printf(PINK"<%s@%s:%s"RESET, pw->pw_name, h_name0,cur_wd);
              for(int i=0;i<cnt_fg;i++){
                   printf(PINK"%s: %d "RESET,fg[i].name,fg[i].time);
              }
                printf(PINK">"RESET);
                cnt_fg=0;
                
                  }
        else{
        printf(PINK"<%s@%s:%s>"RESET, pw->pw_name, h_name0,cur_wd);}
        char* command = (char*)malloc(1000*sizeof(char));
        // char args[1000];
        // strcpy(args, cwd);
         char input[1100]={0};
        if (fgets(input, sizeof(input), stdin) != NULL) {
            
        
            int count=0;
            while(input[count]==' ') count++;
            if(input[count]=='\n') continue;
            // if(strlen(input)==0) continue;
             if(ch_amper(input)) {
                // printf("tehhhh\n");
                // tok(input); }
             }
             else{
                printf(RED"Error: Syntax Error\n"RESET);
                continue;
             }
            char ip2[2048]={0};
            int cnt=0;
            for(int i=0;i<strlen(input);i++)
            {
                if(input[i]=='\n') continue;
                if(input[i]=='&')
                {
                    ip2[cnt++]='&';
                    ip2[cnt++]=';';


                }
                else{
                ip2[cnt++]=input[i];}
            }
            ip2[cnt]='\0';
            input[strcspn(input, "\n")] = 0;
            // printf("input: %s\n", ip2);
            // input[strlen(input)-1]='\0';
            // printf("input: %s\n", input);
            char copy[1024];
            strcpy(copy,input);
            // printf("copy: %s\n", copy);
            // if(strcmp(input,"log")!=0)  {
            //     logentr(input);}
            
           tok(ip2);
          if(!is_log)  {
            log_entry(copy);
            }
           if(is_log) is_log=0;
        //  if   char* token = strtok(input,";" );
        //     while(token!=NULL)
        //     {
        //          while (*token == ' ') token++;
        //     char* end = token + strlen(token) - 1;o
        //     while (end > token && *end == ' ') end--;
        //     *(end + 1) = '\0';
        //          printf("token: %s\n", token);
                
        //         if (sscanf(token, "%s %[^\n]", command, args) == 1) {
        //                 strcpy(args, cwd);
        //             }
        //         if(strcmp(command,"hop")==0)
        // {
        //     //  printf("args: %s\n", args);   
        //     // printf("%s",args);
        //    char buff[1000];

        //    char *result = hop(args, cwd, cur_wd);
        //    if(result == NULL)
        //    {
        //        printf("Error in changing directory\n");
        //    }
        //    else
        //    {
        //        strcpy(cur_wd, result);
        //    }
        // }
        // else {
        //      printf("sys_call\n");
        //      sys_call(token);
        // }
        //         token = strtok(NULL,";");
        //     }
            // Parse the input
           
        // 

      
       
        // printf("%s",args);
        
    }
    else{
         if (feof(stdin)) {
            printf("\n Exiting...\n");
            kill_all();
            // free(command);
            break;
        } 
    }

}
  return 0;
}