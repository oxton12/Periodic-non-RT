#include <ThreadStopwatchArgs.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <evl/sched.h>
#include <sys/mman.h>

class ThreadStopwatch {
 public:
  ThreadStopwatch(int iterations, int calcIterations, bool recordJitter,
                  const evl_sched_attrs* attr);

  void runTest(void* (*loaderFunc)(void*));

 private:
  int iterations_;
  int calcIterations_;
  bool recordJitter_;
  const evl_sched_attrs* attr_;

  void outputResult(long* result);
  void saveResult(long* result);
};
