
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
char* qu[15];
  int l=0;
  int r=0;
  int cnt;
// #include <sys/wait.h>
char u_name[256];
 char h_name0[256];
  char cwd[1024];
   char cur_wd[1000];
   char prev_wd[1000];
   int is_log=0;
   fgproc fg[1024];
   int cnt_fg=0;
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
           int done=0;
        //    char args[1000];
           char b[1000];
           char cpy[1024];
           strcpy(cpy,token);
                            getcwd(b,sizeof(b));
        while (*token == ' ') token++;
            char* end = token + strlen(token) - 1;
            while (end > token && *end == ' ') end--;
            *(end + 1) = '\0';
           if(strncmp(token,"hop",3)==0)
           {
                
                 char* result;
                  char args[1000];
                   char command[1000];
                if(strcmp(token,"hop")==0){
                     hop(cwd,cwd,cur_wd,0);
                }
                else{
                 sscanf(token,"%s %[^\n]",command,args);  
                 char arg2[1000];
                 strcpy(arg2,args);
                 helper(arg2, cwd, cur_wd,0);
                }
                  
        //    if(result == NULL)
        //    {
        //        printf(RED"Error in changing directory\n"RESET);
        //    }
        //    else
        //    {
        //        strcpy(cur_wd, result);
        //    }
           }
              else if(strncmp(token,"reveal",6)==0){
                     if(strcmp(token,"reveal")==0){
                         main_function(b,b,cwd,0,cpy);
                     }
                     else{
                         char args[1000];
                         char command[1000];
                         
                         sscanf(token,"%s %[^\n]",command,args);
                         main_function(args,b,cwd,0,cpy);
                     }
              }
                else if(strncmp(token,"seek",4)==0)
                {
                    if(strcmp(token,"seek")==0){
                        printf("Error: No arguments provided\n");
                    }
                    else{
                        char args[1000];
                        char command[1000];
                        sscanf(token,"%s %[^\n]",command,args);
                        seek(args,b,cwd,0);
                    }
                }
                else if(strncmp(token,"log",3)==0 && done==0)
                {
                    if(strcmp(token,"log")==0){
                        log_print();
                        is_log=1;
                        done=1;
                    }
                    else{
                         is_log=1;
                         char args[1000];
                        char command[1000];
                        sscanf(token,"%s %[^\n]",command,args);

                         log_call(args);
                    }
                  
                }
                else if(strncmp(token,"proclore",8)==0)
                {
                     proclore(cpy);
                }
                else {
                    if(done==0){
                    char buff[1000];
                    strcpy(buff, token);
                    sys_call(buff);}
                }
            token=strtok_r(NULL,";",&save);
        }
}
int main()
{
  
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
         char input[1100];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            int count=0;
            while(input[count]==' ') count++;
            if(input[count]=='\n') continue;
            // if(strlen(input)==0) continue;
            char ip2[2048];
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

}
}