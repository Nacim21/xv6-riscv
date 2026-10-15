#include "kernel/types.h"
#include "user/user.h"

#define WORK 300000000UL

void spin(void) {
  for(volatile uint64 i = 0; i < WORK; i++) ;
}

int main(void)
{
  int prios[] = {1, 5, 10};
  setpriority(20);   // parent highest so it finishes forking first

  for(int i = 0; i < 3; i++){
    if(fork() == 0){
      setpriority( prios[i]);
      int start = uptime();
      spin();
      printf("pid %d prio %d done: start %d end %d\n",
             getpid(), prios[i], start, uptime());
      exit(0);
    }
  }
  for(int i = 0; i < 3; i++) wait(0);
  exit(0);
}
