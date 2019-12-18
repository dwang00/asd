# Shell
David Wang, Systems Level Programming, PD 5, Project 1

make test - tests commands from tests.txt  
make run - runs shell

**Features**
1. can run command line commands, multiple commands on the same line should be
   separated by ";"

2. can redirect inputs and outputs using "<" and ">"

3. can connect 2 commands with pipes "|"

# Limitations
1. redirect output and overwrite ">>" not implemented

2. piping limited to single pipe

# Headers
* `char **parsein(char * input, char * delim);)`
  - inputs: char * input, char * delim
  - parses through char pointer "input"
  - returns array of pointers, values separated by char "delim"
* `int oredirect(char * input);`
  - inputs: char * input
  - if ">" is detected in input, output redirection will be run on the given
    parameters
  - returns 1
* `int iredirect(char * input);`
   - inputs: char * input
   - if "<" is detected in input, input redirection will be run on the given
     parameters
   - returns 1
*  `int oiredirect(char * input)`
   - inputs: char * input
   - if ">" and "<" is detected in input, output and input redirection will be
     run on the given parameters
   - returns 1
 * `int newpipe (char * input)`
   - inputs: char * input
   - if "|" is detected in input, the two commands will be run such that the
     output of the first command will be use as a parameter for the 2nd
   - returns 1
 * `int intsigs(char * input);`
   - inputs: char * input,
   - returns signaling ints for use in main.c
