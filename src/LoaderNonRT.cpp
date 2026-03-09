#include <LoaderNonRT.h>
#include <ThreadStopwatchArgs.h>

void* loaderNonRT(void* arg) {
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

  struct timespec start, current, next_time;

  clock_gettime(CLOCK_MONOTONIC, &start);
  clock_gettime(CLOCK_MONOTONIC, &next_time);

  for (int i = 0; i < iterations; i++) {
    next_time.tv_nsec += 1e6;

    if (next_time.tv_nsec >= 1e9) {
      next_time.tv_sec++;
      next_time.tv_nsec -= 1e9;
    }

    piApprox(calcIterations);

    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_time, NULL);

    if (recordJitter) {
      clock_gettime(CLOCK_MONOTONIC, &current);
      result[i] = (current.tv_sec - start.tv_sec) * 1e9 +
                  (current.tv_nsec - start.tv_nsec);
      clock_gettime(CLOCK_MONOTONIC, &next_time);
      clock_gettime(CLOCK_MONOTONIC, &start);
    }
  }

  if (!recordJitter) {
    clock_gettime(CLOCK_MONOTONIC, &current);
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
