#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>




int count(char* line, char * c);

char **parsein(char* line, char * delim);

int exec_cmd(char *cmd);

int find_redirect(char * line);

int output(char * line);

int inputt(char * line);

int mypipe (char * line) ;

int doubleRedirect(char * line);
// char *erase(char *p, char * c);
