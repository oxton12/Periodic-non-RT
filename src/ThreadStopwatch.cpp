#include "ThreadStopwatch.hpp"

ThreadStopwatch::ThreadStopwatch(int iterations, int calcIterations,
                                 bool recordJitter, const evl_sched_attrs* attr)
    : iterations_(iterations),
      calcIterations_(calcIterations),
      recordJitter_(recordJitter),
      attr_(attr) {}

void ThreadStopwatch::runTest(void* (*loaderFunc)(void*)) {
  pthread_t thread;

  long* result;
  if (recordJitter_)
    result = (long*)malloc(sizeof(long) * iterations_);
  else
    result = (long*)malloc(sizeof(long));
  if (!result) {
    perror("Memory allocation failed");
    return;
  }

  StopwatchArgs stopwatchArgs{iterations_, calcIterations_, recordJitter_,
                              result};

  if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
    perror("mlockall");
    exit(EXIT_FAILURE);
  }

  pthread_create(&thread, attr_, loaderFunc, &stopwatchArgs);
  pthread_join(thread, NULL);

  /*   if (!result) {
      fprintf(stderr, "Thread failed\n");
      return;
    } */

  outputResult(result);

  free(result);
}

void ThreadStopwatch::outputResult(long* result) {
  double duration_ms = 0;
  if (recordJitter_) {
    for (int i = 0; i < iterations_; i++) {
      duration_ms += result[i] / 1e6;
    }
  } else
    duration_ms = *result / 1e6;

  double delay = duration_ms - iterations_;
  double average_delay = delay / iterations_;

  printf(
      "Expected duration: %d ms \nReal duration: %f ms \nDelay: %f ms "
      "\nAvg "
      "delay: "
      "%f ms\n",
      iterations_, duration_ms, delay, average_delay);

  if (recordJitter_) saveResult(result);
}

void ThreadStopwatch::saveResult(long* result) {
  FILE* file = fopen("jitter.txt", "w");
  if (file == NULL) {
    free(result);
    perror("Error opening file");
  }

  for (int i = 0; i < iterations_; i++) {
    fprintf(file, "%ld\n", result[i]);
  }

  fclose(file);
}
