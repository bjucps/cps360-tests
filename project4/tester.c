// Do not modify this file. It will be replaced by the grading scripts
// when checking your project.

#include "types.h"
#include "stat.h"
#include "user.h"

lock_t lock;

void my_thread(void *p1, void *p2)
{

  printf(1, "Running thread %s", (char*)p1);
  for (int i = 0; i < 10; ++i) {
    sleep(50);
    printf(1, "Thread %s i = %d\n", (char*)p1, i);
  }
  lock_acquire(&lock);
  printf(1, "%s in critical section", (char*)p1);
  lock_release(&lock);
}

int
main(int argc, char *argv[])
{
  printf(1, "%s", "** Thread Test Program **\n");
  lock_init(&lock);
  int result = thread_create(my_thread, "threadA", NULL);
  printf(1, "Got %d from thread_create", result);
  result = thread_create(my_thread, "threadB", NULL);
  printf(1, "Got %d from thread_create", result);
  result = thread_join();
  printf(1, "Got %d from thread_join", result);
  result = thread_join();
  printf(1, "Got %d from thread_join", result);
  result = thread_join();
  printf(1, "Got %d from thread_join, expecting -1", result);

  exit();
}
