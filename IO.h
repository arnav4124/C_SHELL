#ifndef IO_H
#define IO_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "hop.h"
#include "seek.h"
#include "reveal.h"
#include "log.h"
#include "proclore.h"
#include "bgprocess.h"
// #include "main.h"
#include <sys/wait.h>
#include <errno.h>
#include "background.h"
#include "ping.h"
// #include "fgprocess.h"
#include "log.h"
#include "fgbg.h"
#include "iman.h"
#include "neo.h"
int   execute_command(char *cmd,char* cur_wd,char* cwd);
#endif