#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
  printf("initial priority: %d\n", getpriority());

  int old = setpriority(5);
  printf("setpriority returned old = %d\n", old);
  printf("new priority: %d\n", getpriority());

  int child = fork();
  if(child == 0){
    char *argv[] = { "ps", 0 };
    exec("ps", argv);
    printf("exec ps failed\n");
    exit(1);
  }
  wait(0);
  exit(0);
}
