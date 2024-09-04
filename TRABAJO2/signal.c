#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char* argv[]){
  int sig = atoi(argv[1);
  pid_t pid = (pid_t)atoi(argv[2]);
  kill(pid,sig);
  
}
