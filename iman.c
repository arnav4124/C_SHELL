#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 8192


void print_without_header(char*line){
    int i=0;
    int flag=0;
    int first_tag=0;
    while(line[i]!='\0'){
        if(line[i]=='<'){
            flag=1;
            first_tag=1;
        }
        if(flag==0 && first_tag==1){
            printf("%c",line[i]);
        }
        if(line[i]=='>'){
            flag=0;
        }
        i++;
    }
}
void iman(char* args) {
    char command_name[256]={0};
    strcpy(command_name,args);
    char sec_no[256]={0};
    int f_brac=0;
    int fl=0;
    for(int i=0;i<strlen(command_name);i++){
        if(command_name[i]=='('){
            fl=1;
            f_brac=i;
        }
        if(fl==1 && command_name[i]>=48 && command_name[i]<=57){
            char buf[2];
            buf[0]=command_name[i];
            buf[1]='\0';
            strcat(sec_no,buf);
        }
        if(command_name[i]==')'){
            break;
        }
    }
    if (fl==1){
        command_name[f_brac]='\0';
    }
    // printf("command_name: %s\n",command_name);
    const char *hostname = "man.he.net";
     char path_template[1024]= "/?topic=%s&section=all";
    
    char path[256];
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    int header_end = 0;

   
    
     if(strlen(sec_no)>0){
        snprintf(path_template, sizeof(path_template), "/?topic=%s&section=%s", command_name,sec_no);
        // snprintf(path, sizeof(path), path_template, command_name,sec_no);
        strcpy(path,path_template);
    }
   
    else{
    snprintf(path, sizeof(path), path_template, command_name);}
    //  printf("path: %s\n",path);
   
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return ;
    }

 
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        return ;
    }

   
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(80);


    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        return ;
    }

 
    snprintf(buffer, sizeof(buffer),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             path, hostname);

    if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
        perror("send");
        return ;
    }

 
   

   
    char html[BUFFER_SIZE] = {0};
    int total_received = 0;
    int fl2=0;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the buffer

        // Write the response to the file
    //    if(buffer[0]=='<' ){
    //        fl2=1;
    //    }

       print_without_header(buffer);
        //   printf("%c\n",buffer[0]);
        total_received += bytes_received;
    }
    if (bytes_received < 0) {
        perror("recv");
       
        close(sockfd);
        return ;
    }

    
    close(sockfd);

   
    
    // while(fgets(line, sizeof(line), file))
    // {
    //     // printf("%d\n",count);
    //    print_without_header(line);
    // }
  

    return;
}


