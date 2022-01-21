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
#define DEBUG
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

// returns TRUE if <file1> contains <contents>
int check_file(char *file1, char *contents) {
  int fd1 = open(file1, O_RDONLY);
  int len = strlen(contents);
  char *buf = malloc(len+2);

  int result = 0;
  if (fd1 == -1) {
    #ifdef DEBUG
    printf(1, "%s not found.\n", file1);
    #endif
    goto done;
  }

  memset(buf, 0, len+2);
  int rc = read(fd1, buf, len+1);
  #ifdef DEBUG
  printf(1, "rc = %d; buf = '%s'\n", rc, buf);
  #endif
  result = (rc == len && strcmp(buf, contents) == 0);

done:
  if (fd1 != -1) close(fd1);

  return result;
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

void fatTests() 
{
  startSection("RAM Disk: FAT Tests");

  check(check_file("/d/hello.txt", "Hello\n"), "FAT File 1 Read");  
  check(check_file("/d/test.txt", "This is a test file\n"), "FAT File 2 Read");  

  check(open("/d/missing", O_RDONLY) == -1, "FAT File Missing Test");

  endSection();
}

int
main(int argc, char *argv[])
{
  fatTests();

  printf(STDOUT, "TOTAL: Passed %d of %d tests\n", totalPassedTests, totalTests);

#ifdef SHELL
  char *SHELL_CMD[] = { "sh", NULL };
  exec(SHELL_CMD[0], SHELL_CMD);
#endif  

  halt();
  
}
