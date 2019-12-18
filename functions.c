#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "functions.h"

char ** parsein(char * input, char * d) {
  int num = 1;
  int i;
  for(i = 0; input[i] != '\0'; i++){
    if(&input[i] != d){
      num++;
    }
  }
  char ** args = malloc(num * sizeof(char *));
  i = 0;
  while(input != NULL && i < (num - 1)) {
   char * holder = strsep(&input, d);
   if (strcmp(d, " ") == 0){
     if(holder[0] != '\0') {
       args[i] = holder;
       i++;
     }
   }
   else{
     args[i] = holder;
     i++;
   }
  }
  args[i] = NULL;
  return args;
}

int oredirect(char * input){
  char ** cmd = parsein(input, ">");
  char ** sender = parsein(cmd[0], " ");
  char ** receiver = parsein(cmd[1], " ");
  int fd = open(receiver[0], O_CREAT | O_WRONLY, 0644);
  dup2(fd, STDOUT_FILENO);
  execvp(sender[0], sender);
  close(fd);
  return 1;
}

int iredirect(char * input){
  char ** cmd = parsein(input, "<");
  char ** sender = parsein(cmd[0], " ");
  char ** receiver = parsein(cmd[1], " ");
  int fd = open(receiver[0], O_RDONLY, 0644);
  dup2(fd, STDIN_FILENO);
  execvp(sender[0], sender);
  close(fd);
  return 1;
}

int oiredirect(char * input){
  char ** a = parsein(input, "<");
  char ** b = parsein(a[0], " ");
  char ** c = parsein(a[1], ">");
  char ** f0 = parsein(c[0], " ");
  char ** f1 = parsein(c[1], " ");
  int fd0 = open(f0[0], O_RDONLY, 0644);
  int fd1 = open(f1[0], O_CREAT | O_WRONLY, 0644);
  dup2(fd0, STDIN_FILENO);
  dup2(fd1, STDOUT_FILENO);
  execvp(b[0], b);
  close(fd0);
  close(fd1);
  return 1;
}

int newpipe (char * input) {
  char ** cmd = parsein(input, "|");
  char ** a = parsein(cmd[0], " ");
  char ** b = parsein(cmd[1], " ");
  int x[2];
  pipe(x);
  int pid = fork();
  if (pid){
    close(x[1]);
    dup2(x[0],0);
    execvp(b[0], b);
    close(x[0]);
  }
  else {
    close(x[0]);
    dup2(x[1],1);
    execvp (a[0], a);
    close(x[1]);
    exit(0);
  }
  return 1;
}

int intsigs(char * input){
  if (strstr(input, "<")) {
    if (strstr(input, ">")) {
      return 1;
    }
    return 2;
  }
  if (strstr(input, ">")) {
    return 3;
  }
  if (strstr(input, "|"))  {
    return 4;
  }
  return 0;
}
