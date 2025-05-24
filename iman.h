#ifndef IMAN_H
#define IMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Define buffer size
// #define BUFFER_SIZE 4096

// Function prototype to fetch man page
void print_without_header(char*line);
void iman(const char *command);

#endif // IMAN_H
