#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

void dump_pinfo()
{
  int rc, i;
  struct pstat ps;
  rc = getpinfo(&ps);
  if (rc < 0) {
    printf(1, "*** ALERT: getpinfo() returned -1\n");
  }
  printf(1, "PID\tTickets\tTicks\n");
  for (i = 0; i < NPROC; ++i) {
    if (ps.inuse[i]) {
      printf(1, "%d\t%d\t%d\n", ps.pid[i], ps.tickets[i], ps.ticks[i]); 
    }
  }

}

int
main(int argc, char *argv[])
{
  int rc;
  struct pstat ps;
  int i;

  int tickets = 0;
  printf(1, "Calling settickets(%d) ... \n", tickets);
  rc = settickets(tickets);  
  printf(1, "settickets(%d) returned: %d\n", tickets, rc);

  tickets = -1;
  rc = settickets(tickets);  
  printf(1, "settickets(%d) returned: %d\n", tickets, rc);

  tickets = 2;
  rc = settickets(tickets);  
  printf(1, "settickets(%d) returned: %d\n", tickets, rc);

  rc = getpinfo(&ps);
  printf(1, "Got %d back from getpinfo\n", rc);
  if (rc == -1)
    return 1;
  printf(1, "\nStatus before children launched:\n");
  dump_pinfo();

  printf(1, "\nLaunching children...\n");
  for (i = 10; i <= 30; i += 10)
    if (fork() == 0) {
      settickets(i);
      printf(1, "child running with %d tickets.\n", i);
      while (1) ;
    }

  sleep(100);
  printf(1, "\nStatus after children launched:\n");
  dump_pinfo();
  printf(1, "Starting regular probes...\n");
  for (int it = 0; it < 5; ++it) {
    rc = getpinfo(&ps);
    if (rc == -1) {
      printf(1, "getpinfo returned -1!\n");
      exit();
    }
    int data[3];
    for (i = 0; i < NPROC; ++i) {
      if (ps.inuse[i] && (ps.tickets[i] % 10 == 0)) {
        // printf(1, "%d,%d\n", it, ps.ticks[i]);
        data[ps.tickets[i] / 10 - 1] = ps.ticks[i];
      }
    }
    printf(1, "%d,%d,%d,%d\n", it, data[0], data[1], data[2]); 

    sleep(500);
  }


  halt();
  exit();
}
