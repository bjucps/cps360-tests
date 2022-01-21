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

// Execute command in argv, placing STDOUT into <output>
void doexec(char *argv[], char *output) {
  int forkresult;

  if ((forkresult = fork()) == -1) { printf(1, "fork() failed"); exit(); }

  if (forkresult == 0) {
    // child
    #ifdef DEBUG
    printf(STDOUT, "Launching %s...\n", argv[0]);
    #endif
    close(1);
    unlink(RESULTFILE);
    open(RESULTFILE, O_CREATE | O_WRONLY);
    exec(argv[0], argv);
    // exec failed
    exit();
  } else {
    output[0] = 0;
    #ifdef DEBUG
    printf(STDOUT, "doexec Waiting...\n");
    #endif
    wait();
    #ifdef DEBUG
    printf(STDOUT, "doexec Reading...\n");
    #endif
    int fd = open(RESULTFILE, O_RDONLY);
    
    memset(output, 0, OUTBUF_SIZE);
    int count = read(fd, output, OUTBUF_SIZE-1);
    close(fd);
    if (count > 0) {
      output[count] = 0;
      // strip newline, if present
      if (output[count-1] == '\n')
        output[count-1] = 0;
    }
    #ifdef DEBUG
    printf(STDOUT, "doexec Received: %s\n", output);
    #endif
  }
  
}

// Returns TRUE if output of doexec(cmdarr) matches expected
int ckexec(char *cmdarr[], char *expected)
{
  doexec(cmdarr, outbuf);
  if (expected)
    return strcmp(outbuf, expected) == 0;
  return 1;
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

void ckcp(char *label, char *src, char *dest, char *stdout, int success)
{
  #ifdef DEBUG
  printf(STDOUT, "Starting test: %s\n", label);
  #endif

  char *CP_TST[] = { "cp", src, dest, NULL };
  int pass = ckexec(CP_TST, stdout);
  if (pass && success) {
    pass = compare_files(src, dest);
  }
  check(pass, label);
  if (!pass && stdout) {
    printf(STDOUT, "  ... received: '%s', expected: '%s' ...\n", outbuf, stdout);
  }
}

void cpTests()
{
  startSection("cp Tests");

  ckcp("cp [echo to /tmp/e]", "echo", "/tmp/e", NULL, 1);
  ckcp("cp [grep to /wc]", "grep", "/wc", NULL, 1);
  mkdir("/foo");
  ckcp("cp [error - to directory]", "grep", "/foo", "Destination may not be a directory.", 0);
  ckcp("cp [error - missing file]", "asdf", "/foo", "Source file does not exist.", 0);

  endSection();
}

void basicRDTests() 
{
  // Test: Create a file in /ram using a single write
  int fd;
  char *file1 = "/ram/f1";
  char buf[80];

  startSection("RAM Disk: Basic Tests");

  check((fd = open(file1, O_CREATE | O_WRONLY)) > 0, "Create RAMDisk File 1");

  check(fd > 0 && write(fd, "abcde", 5) == 5, "Write to RAMDisk File 1");
  close(fd);

  check((fd = open(file1, O_RDONLY)) > 0, "Open RAMDisk File 1");

  memset(buf, 0, sizeof(buf));
  check(fd > 0 && read(fd, buf, 7) == 5 && strcmp(buf, "abcde") == 0, "Read from RAMDisk File 1");
  close(fd);

  // Test: Create another file in /ram using multiple writes and reads
  char *file2 = "/ram/f2";
  check((fd = open(file2, O_CREATE | O_WRONLY)) > 0, "Create RAMDisk File 2");

  if (fd > 0) {
    for (int i = 0; i < 5; ++i) {
      char ch = 'a' + i;
      write(fd, &ch, 1);
    }
    close(fd);
  }

  memset(buf, 0, sizeof(buf));
  fd = open(file2, O_RDONLY);
  if (fd > 0) {
    for (int i = 0; i < 5; ++i) {
      read(fd, buf + i, 1);
    }
    close(fd);
  }
  check(fd > 0 && strcmp(buf, "abcde") == 0, "Multi-Read from RAMDisk File 2");

  // Test: Verify first file still readable
  fd = open(file1, O_RDONLY);
  check(fd > 0 && read(fd, buf, 7) == 5 && strcmp(buf, "abcde") == 0, "Read from RAMDisk File 1");

  // Error handling test: Open a nonexistent file in /ram
  check(open("/ram/abqrre", O_RDONLY) == -1, "Open nonexisting RAM file");
  
  endSection();
}

void moreRDTests()
{

  // Test: Overwrite a file in /ram using a single write
  int fd;
  char *file1 = "/ram/f1";
  char buf[80];

  startSection("RAM Disk: More Tests");

  // Create previously created RAMDisk File
  check((fd = open(file1, O_CREATE | O_WRONLY)) > 0, "Re-create RAMDisk File 1");

  check(fd > 0 && write(fd, "edcba", 5) == 5, "Write to RAMDisk File 1");
  close(fd);

  check((fd = open(file1, O_RDONLY)) > 0, "Open RAMDisk File 1");

  memset(buf, 0, sizeof(buf));
  check(fd > 0 && read(fd, buf, 7) == 5 && strcmp(buf, "edcba") == 0, "Read from RAMDisk File 1");
  close(fd);

  endSection();

}

int
main(int argc, char *argv[])
{
  mkdir("/tmp");
  cpTests();
  basicRDTests();
#ifdef MORERDTESTS
  moreRDTests();
#endif

  printf(STDOUT, "TOTAL: Passed %d of %d tests\n", totalPassedTests, totalTests);

#ifdef SHELL
  char *SHELL_CMD[] = { "sh", NULL };
  exec(SHELL_CMD[0], SHELL_CMD);
#endif  

  halt();
  
}
