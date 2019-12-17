#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"



int main(){

  while(1) {
    char dir[4096];
      char input[4096];
    // gets the current directory
    // getcwd(dir, sizeof(dir));
    // printf("%s$ ", dir);
    fgets(input, sizeof(input), stdin);
    getcwd(dir, sizeof(dir));
   printf("%s$ %s", dir, input);
    if (input[strlen(input) - 1] == '\n') input[strlen(input) - 1] = '\0';
    // getcwd(dir, sizeof(dir));
    // printf("%s$  %s", dir, input);
    char ** args = parse_args(input, ";");
    int i = 0;
    //iterates through args until it reaches the NULL
    while(args[i]){
    int redirect = find_redirect(args[i]);
    // printf("%d\n",  redirect);

    //stout
    if(redirect == 1){
      int result = fork();
      if (result) {
          int status;
          wait(&status);
        } else {
          output(args[i]);
        }
    }

    //stdin
    if (redirect == 2) {
      int result = fork();
      if (result) {
          int status;
          wait(&status);
        } else {
           inputt(args[i]);
        }

    }

    //pipe
    if (redirect == 3) {
      int result = fork();
      if (result) {
          int status;
          wait(&status);
        } else {
          mypipe(args[i]);
        }
    }

    if (redirect == 4) {
      int result = fork();
      if (result) {
          int status;
          wait(&status);
        } else {
          doubleRedirect(args[i]);
        }
    }


    //other command
    if (redirect == 0) {
    char ** command = parse_args(args[i], " ");
    //exits the program
    if (strcmp(command[0], "exit") == 0) {
      return 0;
    }
    // runs cd
    if (strcmp(command[0], "cd") == 0) {
      if (command[2] != NULL){
        printf("Syntax error: cd <filepath> \n" );
      }
      else{
        // enters new directory
        chdir(command[1]);
        if (errno){
          printf("%s\n", strerror(errno));
          errno = 0;
        }
      }
    }
    else {
    int result = fork();
    if (result) {
      //parent
        int status;
        wait(&status);
      } else {
        //runs child
        execvp(command[0], command);
        if (errno) {
          printf("%s: command not found\n", command[0]);
          return 0;
       }
      }
    }
    // i++;
  }
  //iterators through the ;
  i++;
}
    free(args);
  }

  return 0;
}
