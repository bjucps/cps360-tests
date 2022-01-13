// Do not modify this file. It will be replaced by the grading scripts
// when checking your project.

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void
panic(char *s)
{
  printf(2, "%s\n", s);
  exit();
}


#define TEST_DATA "aaaaabaacc"

int
main(int argc, char *argv[])
{
  int p[2];
  char *cmd1[] = { "zip", NULL };
  char *cmd2[] = { "unzip", NULL };

  int fd = open("small", O_CREATE|O_RDWR);
  write(fd, TEST_DATA, strlen(TEST_DATA));
  close(fd);

  printf(1, "\nTesting zip and unzip ... ");
  if(pipe(p) < 0)
      panic("pipe");
  // inspiration for the following came from runcmd() in sh.c
  if(fork() == 0){
    close(0);
    open("small", O_RDONLY); // zip reads from this
    close(1);
    dup(p[1]);  // and writes to the pipe
    close(p[0]);
    close(p[1]);
    exec(cmd1[0], cmd1);
    exit();
  }
  if(fork() == 0){
    close(0);
    dup(p[0]); // unzip reads from this and writes to stdout
    close(1);
    open("output",  O_CREATE|O_RDWR); // and writes to this file
    close(p[0]);
    close(p[1]);
    exec(cmd2[0], cmd2);
  }
  close(p[0]);
  close(p[1]);
  wait();
  wait();

  fd = open("output", O_RDONLY);
  char buf[11];
  if (read(fd, buf, 11) == 10) {
    buf[10] = '\0';
    if (strcmp(buf, TEST_DATA) == 0) {
      printf(1, "PASS\n");
    } else {
      printf(1, "FAIL - wrong output: %s", buf);
    }
  } else {
    printf(1, "FAIL - incorrect number of bytes produced");
  }
  printf(1, "\n\n");

  exit();
}
