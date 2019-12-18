#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functions.h"



int main(){
  while(1) {
    char dir[4096];
    char input[4096];
    fgets(input, sizeof(input), stdin);
    getcwd(dir, sizeof(dir));
    printf("%s$ %s", dir, input);
    if (input[strlen(input) - 1] == '\n') input[strlen(input) - 1] = '\0';
    char ** args = parsein(input, ";");
    int i = 0;
    while (args[i]) {
      int sig = intsigs(args[i]);
      if (sig == 1) {
        int result = fork();
        if (result) {
          int status;
          wait(&status);
        }
        else {
          oiredirect(args[i]);
        }
      }
      if (sig == 2) {
        int result = fork();
        if (result) {
          int status;
          wait(&status);
        }
        else {
          iredirect(args[i]);
        }
      }
      if(sig == 3){
        int result = fork();
        if (result) {
          int status;
          wait(&status);
        }
        else {
          oredirect(args[i]);
        }
      }
      if (sig == 4) {
        int result = fork();
        if (result) {
          int status;
          wait(&status);
        }
        else {
          newpipe(args[i]);
        }
      }
      if (sig == 0) {
        char ** cmd = parsein(args[i], " ");
        if (strcmp(cmd[0], "exit") == 0) {
          return 0;
        }
        if (strcmp(cmd[0], "cd") == 0) {
          if (cmd[2] != NULL){
            printf("cd <filepath> error\n" );
          }
          else{
            chdir(cmd[1]);
            if (errno){
              printf("%s\n", strerror(errno));
              errno = 0;
            }
          }
        }
        else {
          int result = fork();
          if (result) {
            int status;
            wait(&status);
          }
          else {
            execvp(cmd[0], cmd);
            if (errno) {
              printf("command doesn't exist: %s\n", cmd[0]);
              return 0;
            }
          }
        }
      }
      i++;
    }
    free(args);
  }
  return 0;
}
