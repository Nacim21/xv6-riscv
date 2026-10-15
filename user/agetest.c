#include "kernel/types.h"
#include "user/user.h"

#define WORK   200000000UL
#define NCHILD 4

struct result { int pid, prio, resp, turn; };

void spin(void) { for (volatile uint64 i = 0; i < WORK; i++) ; }

int main(void)
{
  int prios[NCHILD] = {10, 10, 10, 0};
  int start[2], res[2];
  pipe(start);
  pipe(res);
  setpriority(99);   // parent highest so it finishes forking first

  for (int i = 0; i < NCHILD; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(1);
    }
    if (pid == 0) {
      setpriority(prios[i]);              // child sets its own priority
      close(start[1]);
      close(res[0]);
      int t0;
      read(start[0], &t0, sizeof(t0));    // wait for "go"
      int first = uptime();               // first time on CPU after go
      spin();
      struct result r = { getpid(), prios[i], first - t0, uptime() - t0 };
      write(res[1], &r, sizeof(r));
      exit(0);
    }
  }

  close(start[0]);
  close(res[1]);
  int t0 = uptime();
  for (int i = 0; i < NCHILD; i++)
    write(start[1], &t0, sizeof(t0));     // release everyone at t0

  int sumresp = 0, sumturn = 0;
  struct result r;
  printf("pid\tprio\tresp\tturnaround\n");
  for (int i = 0; i < NCHILD; i++) {
    read(res[0], &r, sizeof(r));
    printf("%d\t%d\t%d\t%d\n", r.pid, r.prio, r.resp, r.turn);
    sumresp += r.resp;
    sumturn += r.turn;
  }
  for (int i = 0; i < NCHILD; i++) wait(0);

  printf("avg response: %d ticks, avg turnaround: %d ticks\n",
         sumresp / NCHILD, sumturn / NCHILD);
  exit(0);
}
