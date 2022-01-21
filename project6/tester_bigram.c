// Project 6 tests

// To run these tests: Copy this into the user folder and
// do make run to start xv6. At the xv6 shell, execute 'tester' to run
// the tests.
// For additional debug output, uncomment the //#define DEBUG line below 

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

//#define SHELL
//#define DEBUG
#define OUTBUF_SIZE 80
#define RESULTFILE "/tstrslt"
#define MORERDTESTS

const int STDOUT = 1;
char outbuf[OUTBUF_SIZE];

int numTests = 0;
int passedTests = 0;

int totalTests = 0;
int totalPassedTests = 0;

void check(int cond, char *msg)
{
  char *result = cond ? "PASS" : "** FAIL **";
  printf(STDOUT, "%s: %s\n", msg, result);
  if (cond)
    ++passedTests;

  ++numTests;
}

void startSection(char *title)
{
  printf(STDOUT, "%s\n-----------------------------------\n", title);
  passedTests = 0;
  numTests = 0;
}

void endSection()
{
  printf(STDOUT, "\nPassed %d of %d tests\n\n", passedTests, numTests);
  totalPassedTests += passedTests;
  totalTests += numTests;
}

void bigTests() 
{
  // Test: Create a big file in /ram 
  int fd;
  char *file1 = "/ram/f1";
  uint size = 2000;
  char *buf = malloc(size);
  int iterations = 10;

  startSection("RAM Disk: Big File Tests");

  for (int i = 0; i < sizeof(buf); ++i) {
    buf[i] = i;
  }

  fd = open(file1, O_CREATE | O_WRONLY);

  int ok = fd > 0;  
  for (int i = 0; i < iterations && ok; ++i) {
    ok = write(fd, buf, sizeof(buf)) == sizeof(buf);
  }
  
  close(fd);

  check(ok, "Create Big RAM File");

  fd = open(file1, O_RDONLY);

  ok = fd > 0;  
  char rdbuf[sizeof(buf)];
  for (int i = 0; i < iterations && ok; ++i) {
    memset(rdbuf, 0, sizeof(rdbuf));
    ok = read(fd, rdbuf, sizeof(rdbuf)) == sizeof(rdbuf);
    if (ok) {
      for (int j = 0; j < sizeof(rdbuf) && ok; ++j) {
        ok = rdbuf[j] == buf[j];
      }
    }
  }

  ok = ok && read(fd, rdbuf, 1) == 0;
  
  close(fd);
  check(ok, "Verify Big RAM File");



  // Test two concurrent big RAM files

  char *cf1 = "/ram/cf1";
  char *cf2 = "/ram/cf2";
  int fd1 = open(cf1, O_CREATE | O_WRONLY);
  int fd2 = open(cf2, O_CREATE | O_WRONLY);

  memset(buf, 'a', size);
  write(fd1, buf, size); // Initial write to file 1

  char c = 'x';
  write(fd2, &c, 1); // Initial write to file 2

  for (int i = 0; i < 5; ++i) { // Now, write a bunch more to file 1
    write(fd1, buf, size);
  }
  close(fd1);
  close(fd2);
  
  fd2 = open(cf2, O_RDONLY);
  int result = read(fd2, buf, size);
  close(fd2);
  check(result == 1 && buf[0] == 'x', "Concurrent big files");

  endSection();
}

int
main(int argc, char *argv[])
{
  bigTests();

  printf(STDOUT, "TOTAL: Passed %d of %d tests\n", totalPassedTests, totalTests);

#ifdef SHELL
  char *SHELL_CMD[] = { "sh", NULL };
  exec(SHELL_CMD[0], SHELL_CMD);
#endif  

  halt();
  
}
