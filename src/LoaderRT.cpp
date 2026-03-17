#include <LoaderNonRT.h>
#include <ThreadStopwatchArgs.h>
#include <evl/clock.h>
#include <evl/mutex.h>
#include <evl/thread.h>

void* loaderRT(void* arg) {
  StopwatchArgs* threadArgs = (StopwatchArgs*)arg;

  int iterations = threadArgs->iterations;
  int calcIterations = threadArgs->calcIterations;
  bool recordJitter = threadArgs->recordJitter;
  long* result = threadArgs->result;

  /*   long* result;
  if (recordJitter)
    result = (long*)malloc(sizeof(long) * iterations);
  else
    result = (long*)malloc(sizeof(long));

  if (!result) return NULL; */

  if (evl_attach_self("rt-worker") < 0) {
    perror("evl_attach_self");
    return NULL;
  }

  struct timespec start, current, next_time;

  evl_read_clock(CLOCK_MONOTONIC, &start);
  evl_read_clock(CLOCK_MONOTONIC, &next_time);

  for (int i = 0; i < iterations; i++) {
    next_time.tv_nsec += 1e6;

    if (next_time.tv_nsec >= 1e9) {
      next_time.tv_sec++;
      next_time.tv_nsec -= 1e9;
    }

    piApprox(calcIterations);

    evl_sleep_until(CLOCK_MONOTONIC, &next_time);

    if (recordJitter) {
      evl_read_clock(CLOCK_MONOTONIC, &current);
      result[i] = (current.tv_sec - start.tv_sec) * 1e9 +
                  (current.tv_nsec - start.tv_nsec);
      evl_read_clock(CLOCK_MONOTONIC, &next_time);
      evl_read_clock(CLOCK_MONOTONIC, &start);
    }
  }

  if (!recordJitter) {
    evl_read_clock(CLOCK_MONOTONIC, &current);
    *result = (current.tv_sec - start.tv_sec) * 1e9 +
              (current.tv_nsec - start.tv_nsec);
  }

  return NULL;
}

double piApprox(int iterations) {
  double pi = 2;
  for (int j = 2; j < iterations * 2 + 1; j += 2) {
    pi *= j * j / ((j - 1) * (j + 1));
  }
  return pi;
}
