#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"

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

int find_redirect(char * input){
  if (strstr(input, "<")) {
    if (strstr(input, ">")) {
      return 4;
    }
    return 2;

  }

    if (strstr(input, ">")) {
      return 1;
    }

    if (strstr(input, "|"))  {
      return 3;
    }
    return 0;
}

int doubleRedirect(char * input){
  char ** cmd = parsein(input, "<");
  char ** left = parsein(cmd[0], " ");
  char ** right = parsein(cmd[1], ">");
  char ** file0 = parsein(right[0], " ");
  char ** file1 = parsein(right[1], " ");
  //int backup = dup(STDIN_FILENO);
  //int backup1 = dup(STDOUT_FILENO);
  int fd = open(file0[0], O_RDONLY, 0644);
  int fd1 = open(file1[0], O_CREAT | O_WRONLY, 0644);
  dup2(fd, STDIN_FILENO);
  dup2(fd1, STDOUT_FILENO);
  execvp(left[0], left);
  close(fd);
  close(fd1);
  //dup2(backup, STDOUT_FILENO);
  //dup2(backup1, STDIN_FILENO);
  return 1;
}
int oredirect(char * input){
  char ** cmd = parsein(input, ">");
  char ** sender = parsein(cmd[0], " ");
  char ** receiver = parsein(cmd[1], " ");
  int fd = open(receiver[0], O_CREAT | O_WRONLY, 0644);
  //int backup = dup(STDOUT_FILENO);
  dup2(fd, STDOUT_FILENO);
  execvp(sender[0], sender);
  //dup2(backup, STDOUT_FILENO);
  close(fd);
  return 1;
}

int iredirect(char * input){
  char ** cmd = parsein(input, "<");
  //char *f = malloc(strlen(cmd[1]) + 1);
  char ** sender = parsein(cmd[0], " ");
  char ** receiver = parsein(cmd[1], " ");
  int fd = open(receiver[0], O_RDONLY, 0644);
  //int backup = dup(STDIN_FILENO);
  dup2(fd, STDIN_FILENO);
  execvp(sender[0], sender);
  //dup2(backup, STDIN_FILENO);
  close(fd);
  //free(filename);
  return 1;

}
int newpipe (char * input) {
  char ** cmd = parsein(input, "|");
  char ** a = parsein(cmd[0], " ");
  char ** b = parsein(cmd[1], " ");
  int pd[2];
  //int backup = dup(0);
  //int backup2 = dup(1);
  pipe(pd);
  int pid = fork();
  if (pid){
    close(pd[1]);
    //backup = dup(0);
    dup2(pd[0],0);
    execvp(right[0], right);
    //dup2(backup,0);
    //close(backup);
    close(pd[0]);
  }
  else {
    close(pd[0]);
    //backup2 = dup(1);
    dup2(pd[1],1);
    execvp (left[0], left);
    //dup2(backup2,1);
    //close(backup2);
    close(pd[1]);
    exit(0);
  }
  return 1;
}
