#pragma  bg
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>
#include "background.h"
void sys_call(char* str);
