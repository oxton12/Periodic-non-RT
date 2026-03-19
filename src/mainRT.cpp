#include <LoaderRT.h>
#include <evl/sched.h>

#include <ThreadStopwatch.hpp>

int main() {
  pthread_t rt_thread;
  cpu_set_t cpu_set;
  CPU_ZERO(&cpu_set);
  CPU_SET(1, &cpu_set);

  pthread_attr_t attr_thread;
  pthread_attr_init(&attr_thread);
  pthread_attr_setaffinity_np(&attr_thread, sizeof(cpu_set_t), &cpu_set);

  ThreadStopwatch stopwatch =
      ThreadStopwatch(10000, 100000, false, &attr_thread);
  stopwatch.runTest(loaderRT);
}