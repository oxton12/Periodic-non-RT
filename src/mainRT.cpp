//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : Running the real-time experiment
//==============================================================================

#include <LoaderRT.h>
#include <evl/sched.h>

#include <ThreadStopwatch.hpp>

int main() {
  int iterations = 10000;  // how many times the function is executed
  int calcIterations =
      150000;  // amount of iterations done inside the executed function
  bool recordJitter = false;  // whether to record jitter or an overall delay

  cpu_set_t cpu_set;  // cpu settings
  CPU_ZERO(&cpu_set);
  CPU_SET(1, &cpu_set);  // choose the EVL core

  pthread_attr_t attr_thread;  // settings of the thread
  pthread_attr_init(&attr_thread);
  pthread_attr_setaffinity_np(&attr_thread, sizeof(cpu_set_t), &cpu_set);

  ThreadStopwatch stopwatch =
      ThreadStopwatch(iterations, calcIterations, recordJitter, &attr_thread);
  stopwatch.runTest(loaderRT);  // run the experiment
}