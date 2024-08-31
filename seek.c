#include "seek.h"
// #include "queue.h"

#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
extern char cur_wd[1000];
extern char prev_wd[1000];
extern char cwd[1000];

void seek(char* args,char*cur,char*par,int is_log)
{
    char dup[1024];
    strcpy(dup,args);
    char* token=strtok(dup," ");
    // char cmd[1024];
    // snprintf(cmd,sizeof(cmd),"seek %[^\n]",args);
    // printf("token : %s\n",token);
    int dfl=0;
    int ffl=0;
    int efl=0;
    char* path;
    char toexe[1024];
    // char tofull[1024];
    // char* tofull;
    int found_files=0;
    int found_dirs=0;
    path=cur;
    int name_done=0;
    // printf("fcd---> %s\n",path);
    int cnt=0;
    char name[1024];
    int found=0;
    while(token!=NULL)
    {
        // printf("token: %s\n",token);
        if(token[0]=='-')
        {
           if(strlen(token)==1 && name_done==1)
           {
               path=prev_wd;
               name_done++;
           }
           else if(name_done!=0){
                printf(RED"Invalid argument\n"RESET);
                return;
           }
           else if(name_done!=1 && strlen(token)==1){
            //   printf("ndon---> %d\n",name_done);
               printf(RED"Invalid argument\n"RESET);
               return;}
               else{
            for(int i=1;i<strlen(token);i++)
            {
                if(token[i]=='d')
                {
                    dfl=1;
                }
                else if(token[i]=='f')
                {
                    ffl=1;
                }
                else if(token[i]=='e')
                {
                    efl=1;
                }
                else{
                    printf(RED"Invalid argument\n"RESET);
                    return;
                }
            }
               }
        }
        else{
          
           if(name_done==2){
               printf(RED"Invalid argument\n"RESET);
               return;}
            if(name_done==1){
                path=token;
                name_done++;
            }
            else if(name_done==0){
            strcpy(name,token);
            name_done++;}
        }
        token=strtok(NULL," ");
       
    }
    if(strcmp(path,"~")==0)
    {
        path=par;
    }
    else if(strcmp(path,"-")==0)
    {
        path=cur;
    }
    if(dfl && ffl)
    {
        printf(RED"Invalid flags!\n"RESET);
        return;
    }
    // printf("name: %s\n",name);
   Queue* q=createQueue();
//    enqueue(q,name);
    enqueue(q,path);
    while(!isQueueEmpty(q))
    {
         char* pth=dequeue(q);
        //  printf("deq ---> %s\n",pth);
        //  printf("path ----> %s\n",pth);
         DIR *dir=opendir(pth);
         char add[1024];
         
        //  printf("path:----------> %s\n",pth);
    if(!dir)
    {
        perror(RED"opendir"RED);
        return;
    }
    struct dirent *entry;
    struct dirent **entries;
    int cnt=0;
    // while((entry=readdir(dir))!=NULL)
    // {
    //     cnt++;
    // }
    rewinddir(dir);
    entries=malloc(cnt*sizeof(struct dirent*));
    cnt=0;
    while((entry=readdir(dir))!=NULL)
    {
        // printf("entered in %s\n",entry->d_name);
        struct stat file_stat;
         char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", pth, entry->d_name);
        // printf("%s\n",full_path);
        if(stat(full_path,&file_stat)==0)
        {
            // printf("succ\n");
            int fl=S_ISDIR(file_stat.st_mode);
            if( fl && strcmp(entry->d_name,".")!=0 && strcmp(entry->d_name,"..")!=0)
            {
            // printf("dname: %s name : %s\n",entry->d_name,name);
              
               if( (strcmp(entry->d_name,name)==0) && ffl!=1){
                // printf("dirdd---> %s\n",full_path);
                found++;
                found_dirs++;
                int ind=0;
                // tofull=full_path;
                strcpy(toexe,full_path);
                for(int i=0;i<strlen(full_path);i++)
                {
                  if(i>=strlen(path))
                  {
                    printf(BLUE"%c"RESET,full_path[i]);
                    // toexe[ind]=full_path[i];
                    // ind++;
                  }
                }
                printf("\n");
               }

            //   printf("pushing--> %s\n",full_path);
              enqueue(q,full_path);
            }
            else if(!fl){
                //  printf("ahhhh---->  ");
                 if(strcmp(entry->d_name,name)==0 && dfl!=1)
                 {
                        // printf("dddd --> %s\n",full_path);
                        // tofull=full_path;
                        found++;
                        found_files++;
                        int ind=0;
                        strcpy(toexe,full_path);
                        for(int i=0;i<strlen(full_path);i++)
                        {
                            if(i>=strlen(path))
                            {
                                printf(GREEN"%c"RESET,full_path[i]);
                                // toexe[ind]=full_path[i];
                                // ind++;
                            }
                        }
                        printf("\n");
                 }
            }

        }
    }
    }
    if(!found)
    {
        printf(RED"No match found\n"RESET);
        return;
    }
    // printf("efl : %d\n found : %d\n file: %d\n",efl,found,found_files);
    if(efl && found==1)
    {
         
         if(found_files){
            printf("File found\n");   
            // char dup[1024];
            // for(int i=1;i<strlen(toexe);i++)
            // {
            //     dup[i-1]=toexe[i];
            // }
            // dup[strlen(tofull)-1]='\0';
             FILE *file=fopen(toexe,"r");
            // printf("File path: %s\n",toexe);
                if(file==NULL)
                {
                   int err=errno;
                   switch(err)
                   {
                       case EACCES:
                       printf(RED"Missing permissions for task!\n"RESET);
                       break;
                       case ENOENT:
                       printf(RED"No such file or directory\n"RESET);
                       break;
                       default:
                       printf(RED"Error in opening file\n"RESET);
                   }
                   return;
                }
                char line[1024];
                int cnt=0;
                while(fgets(line,sizeof(line),file))
                {
                    printf("%s",line);
                    cnt++;
                }
                // printf("Lines: %d\n",cnt);
                fclose(file);
         }
            else if(found_dirs){
              
              char copy[1024];
              getcwd(copy,sizeof(copy));
            //   printf("prev dir---->%s\n",copy);
              if( chdir(toexe)!=0){
                  
                //    printf()
                   int err=errno;
                     switch(err)
                     {
                          case EACCES:
                          printf(RED"Missing permissions for task!\n"RESET);
                          break;
                          case ENOENT:
                          printf(RED"No such file or directory\n"RESET);
                          break;
                          default:
                          printf(RED"Error in changing directory\n"RESET);
                          return;
                     }
              }
              char buff[1024];
                 strcpy(prev_wd, copy);
                getcwd(buff,sizeof(buff));
                int  cnt_cwd=sl_cntr(par);
                int cnt_buff=sl_cntr(buff);
                if(strcmp(buff,par)==0){
                     strcpy(cur_wd,"~");
                }
                else if(cnt_buff>cnt_cwd)
                {
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
                }
                else{
                    strcpy(cur_wd,buff);
                }

                

                // cur
            }

    }
    

}