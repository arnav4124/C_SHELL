#include "hop.h"
char p_wd[1000];
extern char prev_wd[1000];
#define ll long long int
ll sl_cntr(char* str)
{
    ll cnt=0;
    for(ll i=0;i<strlen(str);i++)
    {
        if(str[i]=='/')
        {
            cnt++;
        }
    }
    return cnt;
}
void helper(char* args,char* cwd,char* cur_wd,int is_log)
{
    char* save3;
    
    char* token3=strtok_r(args," ",&save3);
    int cnt=0;
   while(token3!=NULL)
   {
         char dupl[1000];
            strcpy(dupl,token3);
         hop(dupl,cwd,cur_wd,is_log);
        //  printf("tokk--->%s   no----> %d\n",token3,cnt++);
            token3=strtok_r(NULL," ",&save3);
   }
   return;
}
void hop(char* args,char* cwd,char* cur_wd,int is_log)
{
     char* copy=(char*)malloc(1000*sizeof(char));
      strcpy(copy,args);
    //   char cmd[1000];
    //   snprintf(cmd, sizeof(cmd), "hop %s", args);
    //   args[strlen(args)-1]='\0';
    //   printf("iiii --->%s\n",args);
      
      
     int fl=0;

     char pr[1000];
     if(strcmp(args,"-")==0)
      {
          fl=1;
          strcpy(args,prev_wd);
      }
      if(strcmp(args,"~")==0)
      {
        //    printf("entered\n");
           strcpy(args,cwd);
      }
      for(int i=0;i<strlen(args);i++)
      {
         if(args[i]==' ')
         {
           
            args[i]='/';
            
         }
      }
    //   printf("args: %s", args);

      getcwd(p_wd,sizeof(p_wd));
     
      int check=chdir(args);
   
             if(check==-1)
             {
                int err=errno;
                if(err==ENOENT)
                {
                    printf("No such file or directory\n");
                }
                else if(err==ENOTDIR)
                {
                    printf("Not a directory\n");
                }
                else if(err==EACCES)
                {
                    printf("Missing permissions for task!\n");
                }
                else{
                    printf("Error in changing directory\n");
                }
                return ;
             }
             else{
                  strcpy(prev_wd,p_wd);
               char buff[1000]={0};
                getcwd(buff, sizeof(buff));
              //  if(fl==1){
              //     strcpy(buff,p_wd);
              //  }
               
                // if(ch1==-1)
                // {
                //       printf("Error in getting current working directory\n");
                //       return;
                // }
                int cnt_cwd=sl_cntr(cwd);
                int cnt_buff=sl_cntr(buff);
                
               if(strcmp(buff,cwd)==0){
                    // printf("1\n");
                     strcpy(cur_wd, "~");
                    getcwd(pr, sizeof(pr));
                    printf("%s\n",pr);
                    
                    // log_entry(cmd);
                     return ;
               }
               else if(cnt_buff>cnt_cwd)
               {
                  int is_substr=1;
                for(int i=0;i<strlen(cwd);i++)
                {
                    if(cwd[i]!=buff[i])
                    {
                        is_substr=0;
                        break;
                    }
                }
                if(is_substr==1){
                //   printf("2\n");
                   char dir[1000];
                   int cnt=0;
                   strcpy(cur_wd, "~");
                  //  printf("buff---->%s\n",buff);
                  //  printf("cwd------>%s\n",cwd);
                   for(int i=strlen(cwd);i<strlen(buff);i++)
                   {
                       dir[cnt]=buff[i];
                      //  printf("%c\n",dir[cnt]);
                       cnt++;
                   }
                   dir[cnt]='\0';
                  //  printf("%s\n",cur_wd);
                  //  printf("dir---->%s   len-->%lld\n",dir,strlen(dir));
                   strcat(cur_wd, dir);
                   getcwd(pr, sizeof(pr));
                    // printf("%s\n",pr);
                   printf("%s\n",buff);
                //   log_entry(cmd);
                   return ;}
                   else{
                    //    printf("4\n");
                       strcpy(cur_wd, buff);
                       printf("%s\n",cur_wd);
                    //    log_entry(cmd);
                       return ;
                   }
               }
               else if(cnt_buff<=cnt_cwd){
                    //  printf("3\n");
                   strcpy(cur_wd, buff);
                   printf("%s\n",cur_wd);
                //    log_entry(cmd);
                   return ;
               }
             }
}