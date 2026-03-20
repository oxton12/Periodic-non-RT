//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A function that measures a delay in execution of a repeating task
// executed every 1 ms in a real-time fashion
//==============================================================================

#include <LoaderNonRT.h>
#include <evl/clock.h>
#include <evl/mutex.h>
#include <evl/sched.h>
#include <evl/thread.h>
#include <stdio.h>

void* loaderRT(void* arg) {
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

  struct evl_sched_attrs
      attr;  // attributes of the thread to set correct schedule policy
  attr.sched_policy = SCHED_FIFO;
  attr.sched_priority = 80;

  // attaching the thread to the EVL core
  if (evl_attach_self("rt-worker") < 0) {
    perror("evl_attach_self");
    return NULL;
  }

  evl_set_schedattr(0, &attr);  // applying schedule policy

  evl_read_clock(EVL_CLOCK_MONOTONIC, &start);
  evl_read_clock(EVL_CLOCK_MONOTONIC, &nextTime);

  for (int i = 0; i < iterations; i++) {
    nextTime.tv_nsec += 1e6;  // when the current iteration must end (in 1 ms)

    // avoid overflow
    while (nextTime.tv_nsec >= 1e9) {
      nextTime.tv_sec++;
      nextTime.tv_nsec -= 1e9;
    }

    incrementinator(calcIterations);  // do work

    evl_sleep_until(EVL_CLOCK_MONOTONIC,
                    &nextTime);  // wait till the time for next iteration comes

    // record delay of this iteration if needed (takes extra time => slightly
    // more delay)
    if (recordJitter) {
      evl_read_clock(EVL_CLOCK_MONOTONIC, &current);
      result[i] = (current.tv_sec - start.tv_sec) * 1e9 +
                  (current.tv_nsec - start.tv_nsec);
      evl_read_clock(EVL_CLOCK_MONOTONIC, &nextTime);
      evl_read_clock(EVL_CLOCK_MONOTONIC, &start);
    }
  }

  // record time it took to perform all the iterations if jitter was not
  // recorded
  if (!recordJitter) {
    evl_read_clock(EVL_CLOCK_MONOTONIC, &current);
    *result = (current.tv_sec - start.tv_sec) * 1e9 +
              (current.tv_nsec - start.tv_nsec);
  }

  return NULL;
}
