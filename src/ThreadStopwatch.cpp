#include "ThreadStopwatch.hpp"

ThreadStopwatch::ThreadStopwatch(int iterations, int calcIterations,
                                 bool recordJitter)
    : iterations_(iterations),
      calcIterations_(calcIterations),
      recordJitter_(recordJitter) {}

void ThreadStopwatch::runTest(void* (*loaderFunc)(void*)) {
  pthread_t thread;
  long* result;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  StopwatchArgs stopwatchArgs{iterations_, calcIterations_, recordJitter_};

  pthread_create(&thread, NULL, loaderFunc, &stopwatchArgs);
  pthread_join(thread, (void**)&result);

  if (!result) {
    fprintf(stderr, "Thread failed\n");
    return;
  }

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
    perror("Error opening file");
    free(result);
  }

  for (int i = 0; i < iterations_; i++) {
    fprintf(file, "%ld\n", result[i]);
  }

  fclose(file);
}
