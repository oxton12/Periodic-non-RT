#include <ThreadStopwatchArgs.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

class ThreadStopwatch {
 public:
  ThreadStopwatch(int iterations, int calcIterations, bool recordJitter);

  void runTest(void* (*loaderFunc)(void*));

 private:
  int iterations_;
  int calcIterations_;
  bool recordJitter_;

  void outputResult(long* result);
  void saveResult(long* result);
};
