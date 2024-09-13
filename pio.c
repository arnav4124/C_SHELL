#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "IO.h"
#include "pio.h"
#define MAX_COMMAND_LENGTH 1024
#define MAX_TOKENS 100
 extern BackgroundProcess* head;
//

#define MAX_COMMAND_LENGTH 1024
#define MAX_TOKENS 100

// Function prototypes for custom commands
// void hop(char *cwd, char *dest, char *cur_wd, int flag);
// void helper(char *args, char *cwd, char *cur_wd, int flag);
// void seek(char *args, char *cur_wd, char *cwd, int flag);k
// void main_function(char *arg, char *cur_wd, char *cwd, int flag, char *cmd);
// void log_print(void);
// void log_call(char *args);
// void proclore(char *args);
// void sys_call(char *cmd);
int check_redir(char* str){
    for(int i=0;i<strlen(str);i++){
        if(str[i]=='>' || str[i]=='<' || str[i]=='|' || str[i]=='\t'){
            return 0;
        }
    }
    return 1;
}
int ch_amper(char* str){
    char dup69[1000]={0};
    strcpy(dup69,str);
    if(dup69[strlen(dup69)-1]=='\n'){
        dup69[strlen(dup69)-1]='\0';
    }
    int l=strlen(dup69);
    while(dup69[l-1]!='|' && l>0){
        l--;
    }
    if(l==0){
        return 1;
    }
    dup69[l]='\0';
    for(int i=strlen(dup69)-1;i>=0;i--){
        if(dup69[i]=='&'){
            return 0;
        }
        if(dup69[i]!=' '&& dup69[i]!='\t' && dup69[i]!='\n' && dup69[i]!='\0'  && dup69[i]!='|'){
            return 1;
        }
    }
    return 0;
}
char* check_alias(char *cmd){
    FILE *fp;
    fp=fopen("/mnt/c/Users/dell/Desktop/mini-project-1-arnav4124/.mysrc","r");
    char line[1000];
    while(fgets(line,1000,fp)!=NULL){
        char *save89;
        char *token89=strtok_r(line,"=",&save89);
        if(strcmp(token89,cmd)==0){
            token89=strtok_r(NULL,"=",&save89);
            return token89;
        }
    }

}
char* check_func(char* cmd1, char* cmd2) {
    FILE *fp = fopen("/mnt/c/Users/dell/Desktop/mini-project-1-arnav4124/.mysrc", "r");
    if (!fp) {
        printf("Error opening file\n");
        return NULL;
    }

    char line[1000];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *save89;
        char *token89 = strtok_r(line, "=", &save89);
        
        if (strcmp(token89, cmd1) == 0) {
            char bu2[1000]={0};
            char* func = (char*)malloc(1000);
            func[0] = '\0';  
            int open_brace = 0;
            while (fgets(bu2, sizeof(bu2), fp) != NULL) {
               if(bu2[0]=='}'){
                   break;
                }
                if(bu2[0]!='{'){
                    strcat(func,bu2);
                    if(bu2[strlen(bu2)-1]=='\n'){
                        func[strlen(func)-1]='\0';
                    }
                    int i=0;
                    for( i=0;i<strlen(func);i++){
                        if(func[i]=='$'){
                           break;
                        }
                    }
                    func[i]='\0';
                    strcat(func,cmd2);
                    strcat(func,";");
                }
                
            }
            fclose(fp);
              // Close the file
              if(func[strlen(func)-1]=='\n'){
                func[strlen(func)-1]='\0';
              }
            return func;  
        }
    }

    fclose(fp);  
    return NULL;
}

void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) { // All spaces?
        *str = 0;
        return;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    *(end + 1) = 0;
}

int execute_command(char *cmd, char *cur_wd, char *cwd) {
    if(cmd[strlen(cmd)-1]=='>' || cmd[strlen(cmd)-1]=='<' || (cmd[strlen(cmd)-1]=='>'&& cmd[strlen(cmd)-2]=='>')){
        printf(RED"Error: Syntax Error\n"RESET);
        return 0;
    }
    char *args[128];
    char *input_file = NULL, *output_file = NULL;
    int append = 0; 
    int is_IO = 0;  

    char *save5;
    char *token5 = strtok_r(cmd, " ", &save5);
    int arg_count = 0;
    while (token5 != NULL) {
        if (strcmp(token5, "<") == 0) {
            token5 = strtok_r(NULL, " ", &save5);
            
            is_IO = 1;
            input_file = token5;
            if(!check_redir(token5)){
                printf(RED"Error: Syntax Error\n"RESET);
                return 0;
            }
            //  if(strcmp(input_file,">")==0||strcmp(input_file,"<")==0||strcmp(input_file,">>")==0|| strcmp(output_file,"\t")==0){
            //     printf(RED"Error: Syntax Error\n"RESET);
            //     exit(0) ;
            // }
        } else if (strcmp(token5, ">") == 0) {
            token5 = strtok_r(NULL, " ", &save5);
            output_file = token5;
            if(!check_redir(token5)){
                printf(RED"Error: Syntax Error\n"RESET);
                return 0;
            }
            // if(strcmp(output_file,">")==0||strcmp(output_file,"<")==0||strcmp(output_file,">>")==0|| strcmp(output_file,"\t")==0){
            //     printf(RED"Error: Syntax Error\n"RESET);
            //     exit(0) ;
            // }
            is_IO = 1;
            append = 0;
        } else if (strcmp(token5, ">>") == 0) {
            token5 = strtok_r(NULL, " ", &save5);
            output_file = token5;
            if(!check_redir(token5)){
                printf(RED"Error: Syntax Error\n"RESET);
                return 0;
            }
            //  if(strcmp(output_file,">")==0||strcmp(output_file,"<")==0||strcmp(output_file,">>")==0|| strcmp(output_file,"\t")==0){
            //     printf(RED"Error: Syntax Error\n"RESET);
            //     exit(0) ;
            // }
            is_IO = 1;
            append = 1;
        } else {
            args[arg_count++] = token5;
        }
        token5 = strtok_r(NULL, " ", &save5);
    }

    // args[

    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);

  
    if (input_file) {
        int in_fd = open(input_file, O_RDONLY);
        if (in_fd < 0) {
            perror("No such input file found!");
            return -1;
        }
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    

    if (output_file) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (append) ? O_APPEND : O_TRUNC;
        int out_fd = open(output_file, flags, 0644);
        if (out_fd < 0) {
            perror("Error opening output file!");
            return -1;
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }
    int done=0;
    if(arg_count==1){
        char cpy[1000]={0};
        strcpy(cpy,args[0]);
        char* check=check_alias(cpy);
        if(check!=NULL){
            char alias[1000]={0};
            strcpy(alias,check);
            // printf("alias: %s\n",check);
            alias[strlen(alias)-1]='\0';
            done=1;
            tok(alias);
        }
    }
    if(arg_count==2){
        char cpy1[1000]={0};
        strcpy(cpy1,args[0]);
        char cpy2[1000]={0};
        strcpy(cpy2,args[1]);
        // printf("cpy1: %s\n",cpy1);
        char* check=check_func(cpy1,cpy2);
        if(check!=NULL){
            char alias[1000]={0};
            strcpy(alias,check);
            // printf("func: %s\n",check);
           if(alias[strlen(alias)-1]=='\n') alias[strlen(alias)-1]='\0';
            done=1;
            tok(alias);
        }
        else{
            // printf("niluu\n");
        }
    }
   
    if(!done){
    if (strcmp(args[0], "hop") == 0) {
        if (arg_count == 1) {
            hop(cwd, cwd, cur_wd, 0);
        } else {
            char arg2[1000] = {0};
            for (int i = 1; i < arg_count; i++) {
                strcat(arg2, args[i]);
                if (i != arg_count - 1) strcat(arg2, " ");
            }
            helper(arg2, cwd, cur_wd, 0);
        }
    } else if (strcmp(args[0], "seek") == 0) {
        if (arg_count == 1) {
            // Handle error: no arguments provided
        } else {
            char arg2[1000] = {0};
            for (int i = 1; i < arg_count; i++) {
                strcat(arg2, args[i]);
                if (i != arg_count - 1) strcat(arg2, " ");
            }
            seek(arg2, cur_wd, cwd, 0);
        }
    } else if (strcmp(args[0], "reveal") == 0) {
        if (arg_count == 1) {
            main_function(cur_wd, cur_wd, cwd, 0, cmd);
        } else {
            char arg2[1000] = {0};
            for (int i = 1; i < arg_count; i++) {
                strcat(arg2, args[i]);
                if (i != arg_count - 1) strcat(arg2, " ");
            }
            main_function(arg2, cur_wd, cwd, 0, cmd);
        }
    } else if (strcmp(args[0], "log") == 0) {
        if (arg_count == 1) {
            log_print();
        } else {
            char arg2[1000] = {0};
            for (int i = 1; i < arg_count; i++) {
                strcat(arg2, args[i]);
                if (i != arg_count - 1) strcat(arg2, " ");
            }
            log_call(arg2);
        }
    } else if (strcmp(args[0], "proclore") == 0) {
        char arg2[1000] = {0};
        for (int i = 0; i < arg_count; i++) {
            strcat(arg2, args[i]);
            if (i != arg_count - 1) strcat(arg2, " ");
        }
        proclore(arg2);
    }
    else if(strcmp(args[0],"neonate")==0){
        if(arg_count==1){
            neonate("5");
        }
        else if(arg_count==3){
            if(strcmp(args[1],"-n")==0){
                neonate(args[2]);
            }
            else{
                printf(RED"Error: Invalid arguments\n"RESET);
            }
        }
        else{
            printf(RED"Error: Invalid arguments\n"RESET);
        }
    }
    else if(strcmp(args[0],"ping")==0){
        if(arg_count==1){
            printf(RED"No arguments"RESET);
        }
        else{
             char arg2[1000] = {0};
             for (int i = 1; i < arg_count; i++) {
                // printf("args[%d]: %s\n",i,args[i]);
                strcat(arg2, args[i]);
                if (i != arg_count - 1) strcat(arg2, " ");
            }
            ping(arg2);
        }
    }
    else if(strcmp(args[0],"activities")==0){
        if(arg_count==1){
            merge_sort(&head);
            print_process();
        }
        else{
            printf(RED"Error: No arguments required\n"RESET);
        }
    }
    else if (strcmp(args[0], "fg")==0){
           if(arg_count==1){
              printf("Error: No arguments provided\n");
           }
              else{
                char arg2[1000]={0};
                for(int i=1;i<arg_count;i++){
                     strcat(arg2,args[i]);
                     if(i!=arg_count-1) strcat(arg2," ");
                }
                fg_call(arg2);
              }
    }
    else if (strcmp(args[0],"bg")==0){
        if(arg_count==1){
            printf("Error: No arguments provided\n");
        }
        else{
            char arg2[1000]={0};
            for(int i=1;i<arg_count;i++){
                strcat(arg2,args[i]);
                if(i!=arg_count-1) strcat(arg2," ");
            }
            bg_call(arg2);
        }
    }
    else if(strcmp(args[0],"iman")==0){
        if(arg_count==1){
            printf("Error: No arguments provided\n");
        }
        else{
            iman(args[1]);

        }
    }
   
     else {
        
        char arg2[1000] = {0};
        for (int i = 0; i < arg_count; i++) {
            strcat(arg2, args[i]);
            if (i != arg_count - 1) strcat(arg2, " ");
        }
        // char* check =chaeck_alias(arg2);
        // printf("sys_call: %s\n", arg2);
        sys_call(arg2);
        sleep(0.2);

    }
   }
    // Restore standard input/output
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);

    close(stdin_backup);
    close(stdout_backup);

    return 1;
}
int check(char* str){
     int after_pipe=0;
     for(int i=0;i<strlen(str);i++){
        //  printf("%c\n",str[i]);

         if(str[i]=='|' && after_pipe==0){
            //  printf(RED"Error: No command before pipe\n"RESET);
             return 0;
         }
         if(str[i]=='|' && after_pipe!=0){
             after_pipe=0;
            continue;
         }

         if(str[i]!=' '&& str[i]!='\n' && str[i]!='|'){
            after_pipe++;
         }
        //  printf("%d\n",after_pipe);
     }
     return after_pipe;
}
void execute_pipeline(char *cmd, char *cwd, char *cur_wd) {
    char cp[1000]={0};
    
    strcpy(cp,cmd);
    int check_valid=check(cp);
    if(!check_valid) {
        printf(RED"Error: Syntax Error\n"RESET);
        return ;
    }
    if(cmd[strlen(cmd)-1]=='|'){
        printf(RED"Error: No command after pipe\n"RESET);
        return ;
    }
    char *commands[MAX_TOKENS];
    int i = 0;
    char *save7;
    char *token7 = strtok_r(cmd, "|", &save7);

    while (token7 != NULL) {
        commands[i] = strdup(token7);
        int check_amper=ch_amper(commands[i]);
        if(!check_amper){
            
            printf(RED"haaError: Syntax Error\n"RESET);
            exit(0);
        }
        if (commands[i] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
        i++;
        token7 = strtok_r(NULL, "|", &save7);
    }
    commands[i] = NULL;

    int num_cmds = i;
    int pipe_fds[2];
    int prev_fd = -1;
    pid_t pids[MAX_TOKENS];  // Array to store child process IDs

    // Backup stdin and stdout
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);

    for (i = 0; i < num_cmds; i++) {
        if (pipe(pipe_fds) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pids[i] == 0) {
            if (prev_fd != -1) {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < num_cmds - 1) {
                dup2(pipe_fds[1], STDOUT_FILENO);
            }
            close(pipe_fds[0]);
            close(pipe_fds[1]);

            char cpy2[1000] = {0};
            strcpy(cpy2, commands[i]);

            // Execute the command
            execute_command(cpy2, cur_wd, cwd);
            // wait(1);
           
            dup2(stdin_backup, STDIN_FILENO);
            dup2(stdout_backup, STDOUT_FILENO);

            exit(EXIT_FAILURE);  
        } else {
            wait(NULL);
            close(pipe_fds[1]);
            if (prev_fd != -1) {
                close(prev_fd);
            }
            prev_fd = pipe_fds[0];
        }
    }
    
    if (prev_fd != -1) {
        close(prev_fd);
    }

   
    for (i = 0; i < num_cmds; i++) {
        waitpid(pids[i], NULL, WUNTRACED);
    }

   
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);

  
    close(stdin_backup);
    close(stdout_backup);

   
    for (i = 0; i < num_cmds; i++) {
        free(commands[i]);
    }
}
