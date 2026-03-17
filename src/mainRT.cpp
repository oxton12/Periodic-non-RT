#include <LoaderRT.h>

#include <ThreadStopwatch.hpp>

int main() {
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(1, &cpuset);
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
  struct sched_param param;
  pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
  param.sched_priority = 42;
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);

  ThreadStopwatch stopwatch = ThreadStopwatch(10000, 20000, false, &attr);
  stopwatch.runTest(loaderRT);
}