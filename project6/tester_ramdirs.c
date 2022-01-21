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

// returns TRUE if files are same
int compare_files(char *file1, char *file2) {
  int fd1 = open(file1, O_RDONLY);
  int fd2 = open(file2, O_RDONLY);
  int result = 0;
  if (fd1 == -1 || fd2 == -1) {
    goto done;
  }

  #ifdef DEBUG
  printf(1, "Comparing %s with %s...\n", file1, file2);
  #endif
  int c1 = 0, c2 = 0;
  int rc1, rc2;
  result = 1;
  while (result && (rc1 = read(fd1, &c1, 1)) == 1) {
    if ((rc2 = read(fd2, &c2, 1)) != 1 || c1 != c2)
      result = 0;
  }
  // should be nothing remaining in file2
  if (result && (read(fd2, &c2, 1) != 0)) {
    result = 0;
  }

done:
  if (fd1 != -1) close(fd1);
  if (fd2 != -1) close(fd2);
  return result;
}

// Execute command in <cmdline>
void doexec(char *cmdline) {
  int forkresult;
  char *argv[10];
  char cmd[80];

  strcpy(cmd, cmdline);

  printf(1, "\n%s\n", cmd);
  for (int i = 0; i < strlen(cmd); ++i) 
    printf(1, "-");
  printf(1, "\n");

  // tokenize cmd
  int arg = 0;
  char *c = cmd;
  argv[0] = cmd;
  while (*c) {
    if (*c == ' ') {
      *c = '\0';
      argv[++arg] = ++c;
    } else 
      ++c;
  }
  argv[++arg] = NULL;

  if ((forkresult = fork()) == -1) { printf(1, "fork() failed"); exit(); }

  if (forkresult == 0) {
    // child
    #ifdef DEBUG
    printf(STDOUT, "Launching %s...\n", argv[0]);
    #endif
    exec(argv[0], argv);
    // exec failed
    exit();
  } else {
    #ifdef DEBUG
    printf(STDOUT, "doexec Waiting...\n");
    #endif
    wait();
  }
  
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

void dirTests() 
{
  startSection("RAM Disk: RAMDisk Directory Tests");

  int fd = open("/ram/f1", O_CREATE | O_WRONLY);
  write(fd, "abcde", 5);
  close(fd);

  // Test 1: List files in /ram
  doexec("cp /ram/f1 /ram/f2");
  doexec("ls /ram");

  // Test 2: Remove a file from /ram
  doexec("rm /ram/f1");
  doexec("ls /ram");

  endSection();
}

int
main(int argc, char *argv[])
{
  dirTests();

  //printf(STDOUT, "TOTAL: Passed %d of %d tests\n", totalPassedTests, totalTests);

#ifdef SHELL
  char *SHELL_CMD[] = { "sh", NULL };
  exec(SHELL_CMD[0], SHELL_CMD);
#endif  

  halt();
  
}
