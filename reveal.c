#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "reveal.h"
#define RESET   "\x1b[0m"
#define WHITE   "\x1b[37m"
#define BLUE    "\x1b[34m"
#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
extern char prev_wd[1000];
// Function to print file permissions
int cmp(const void *const A, const void *const B)
{
    return strcmp((*(struct dirent **) A)->d_name, (*(struct dirent **) B)->d_name);
}
void print_permissions(mode_t mode) {
    printf("%c", (S_ISDIR(mode)) ? 'd' :
                 (S_ISLNK(mode)) ? 'l' :
                 (S_ISREG(mode)) ? '-' : '?');
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
    printf(" ");
}

// Function to print file information
void print_file_info(const char *path, struct dirent *entry, int show_long) {
    if (show_long) {
        struct stat file_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == 0) {
            print_permissions(file_stat.st_mode);
            printf("%ld ", file_stat.st_nlink);
            struct passwd *pw = getpwuid(file_stat.st_uid);
        struct group *gr = getgrgid(file_stat.st_gid);
        printf("%-8s %-8s ", pw->pw_name, gr->gr_name);
            printf("%ld ", file_stat.st_size);
            char time_str[20];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf("%s ", time_str);
        }
    }
      struct stat file_stat;
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
      if (stat(full_path, &file_stat) == 0){
       if (S_ISDIR(file_stat.st_mode))   printf(BLUE "%s \n"RESET, entry->d_name );
       else if (file_stat.st_mode & S_IXUSR) printf(GREEN "%s\n "RESET, entry->d_name );
       else printf(RED "%s \n" RESET, entry->d_name );
        

     }
}

// Function to list directory contents
void list_directory(const char *path, int show_all, int show_long,char* cmd) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror(RED"opendir"RESET);
        return;
    }

    struct dirent *entry;
    struct dirent **entries;
    int entry_count = 0;
    int i;

    // Count entries
    while ((entry = readdir(dir)) != NULL) {
        entry_count++;
    }
    closedir(dir);

    // Allocate memory for entries
    entries = malloc(entry_count * sizeof(struct dirent *));
    if (!entries) {
        perror(RED"malloc"RESET);
        return;
    }

    // Read entries again
    dir = opendir(path);
    entry_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        entries[entry_count] = malloc(sizeof(struct dirent));
        if (!entries[entry_count]) {
            perror(RED"malloc"RESET);
            closedir(dir);
            for (i = 0; i < entry_count; i++) {
                free(entries[i]);
            }
            free(entries);
            return;
        }
        memcpy(entries[entry_count], entry, sizeof(struct dirent));
        entry_count++;
    }
    closedir(dir);

    // Sort entries
    qsort(entries, entry_count, sizeof(struct dirent *), cmp);

    // Print entries
    for (i = 0; i < entry_count; i++) {
        if (!show_all && entries[i]->d_name[0] == '.') {
            continue; // Skip hidden files if -a is not set
        }
        print_file_info(path, entries[i], show_long);
        free(entries[i]);
    }
    // log_entry(cmd);
    free(entries);
}

// Main function
void main_function(char* args,char* cur_wd,char* cwd,int is_log,char* cmd) {
    // char args[256];
    int show_all = 0;
    int show_long = 0;
    char *path = ".";
    char copy[1000];
    strcpy(copy,args);
//  printf("cwd: %s\n",cwd);
    // Read input from user
    // printf("Enter arguments: ");
    // scanf("%255s", args);

    // Parse arguments
    // printf("args: %s\n",args);
    // if (args[0] == '-') {
    //     for (int i = 1; i < strlen(args); i++) {
    //         if (args[i] == 'a') {
    //             show_all = 1;
    //         } else if (args[i] == 'l') {
    //             show_long = 1;
    //         }
    //         else{
    //             printf("Invalid argument\n");
    //             return;
    //         }
    //     }
    // } else {
    //     path = args;
    // }
    // if(strcmp(path,"~")==0)
    // {
    //     path=cwd;
    // }
    // else if(strcmp(path,"-")==0)
    // {
    //     path=cur_wd;
    // }
    char* save1;
    char* token2 = strtok_r(copy, " ",&save1);
    int path_done=0;
    char t_cpy[1000];
    strcpy(t_cpy,token2);
    while(token2!=NULL)
    {
        //  printf("token: %s\n",token);
         
         if(token2[0]=='-' && strlen(token2)>1 )
        {
           if(path_done){
                printf(RED"Invalid argument\n"RESET);
                return;
           }
            for(int i=1;i<strlen(token2);i++)
            {
                if(token2[i]=='a')
                {
                    show_all=1;
                }
                else if(token2[i]=='l')
                {
                    show_long=1;
                }
                else{
                    printf(RED"Invalid argument\n"RESET);
                    return;
                }
            }
        }
        else{
            path_done=1;
           if(strcmp(token2,"~")==0)
           {
               path=cwd;
           }
           else if(strcmp(token2,"-")==0)
           {
                // printf("prev_wd: %s\n",prev_wd);
                path=prev_wd;
           }
           else{
               path=token2;
           }
        }
        strcpy(t_cpy,token2);
        token2=strtok_r(NULL," ",&save1);
    }
    // printf("path: %s\n",path);

    // List directory contents
    list_directory(path, show_all, show_long,cmd);
}

// int main() {
//     main_function();
//     return 0;
// }