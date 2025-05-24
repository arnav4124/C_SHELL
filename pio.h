#ifndef PIO_H
#define PIO_H
void trim_whitespace(char *str);
void execute_pipeline(char* cmd ,char *cwd,char* cur_wd);
#include "IO.h"
int ch_amper(char* str);
#endif