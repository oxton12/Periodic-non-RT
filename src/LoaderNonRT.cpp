//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A function that measures a delay in execution of a repeating task
// executed every 1 ms in a non-real-time fashion
//==============================================================================

#include <LoaderNonRT.h>

void* loaderNonRT(void* arg) {
  StopwatchArgs* threadArgs =
      (StopwatchArgs*)arg;  // settings of the experiments

  int iterations =
      threadArgs->iterations;  // how many times the function is executed
  int calcIterations =
      threadArgs->calcIterations;  // amount of iterations done inside the
                                   // executed function
  bool recordJitter =
      threadArgs->recordJitter;  // whether to record jitter or an overall delay
  long* result = threadArgs->result;  // pointer to the stored result

  struct timespec start, current,
      nextTime;  // timestamps used to measure the delay

  clock_gettime(CLOCK_MONOTONIC, &start);
  clock_gettime(CLOCK_MONOTONIC, &nextTime);

  for (int i = 0; i < iterations; i++) {
    nextTime.tv_nsec += 1e6;  // when the current iteration must end (in 1 ms)

    // avoid overflow
    if (nextTime.tv_nsec >= 1e9) {
      nextTime.tv_sec++;
      nextTime.tv_nsec -= 1e9;
    }

    incrementinator(calcIterations);  // do work

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &nextTime,
                    NULL);  // wait till the time for next iteration comes

    // record delay of this iteration if needed (takes extra time => slightly
    // more delay)
    if (recordJitter) {
      clock_gettime(CLOCK_MONOTONIC, &current);
      result[i] = (current.tv_sec - start.tv_sec) * 1e9 +
                  (current.tv_nsec - start.tv_nsec);
      clock_gettime(CLOCK_MONOTONIC, &nextTime);
      clock_gettime(CLOCK_MONOTONIC, &start);
    }
  }

  // record time it took to perform all the iterations if jitter was not
  // recorded
  if (!recordJitter) {
    clock_gettime(CLOCK_MONOTONIC, &current);
    *result = (current.tv_sec - start.tv_sec) * 1e9 +
              (current.tv_nsec - start.tv_nsec);
  }

  return NULL;
}
