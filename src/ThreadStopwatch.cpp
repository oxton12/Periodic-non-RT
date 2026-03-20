//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A class used to prepare for measuring the delay (or jitter) of a
// peridically executed function, run an experiment, output and save the
// results. Can be used for both real-time and non-real-time experiments.
//==============================================================================

#include "ThreadStopwatch.hpp"

ThreadStopwatch::ThreadStopwatch(int iterations, int calcIterations,
                                 bool recordJitter, const pthread_attr_t* attr)
    : iterations_(iterations),
      calcIterations_(calcIterations),
      recordJitter_(recordJitter),
      attr_(attr) {}

void ThreadStopwatch::runTest(void* (*loaderFunc)(void*)) {
  pthread_t thread;  // thread variable

  long* result;  // pointer to the experiment results
  // allocate memory to delay of every iteration if recordJitter_ is true, else
  // only for the overall delay
  if (recordJitter_)
    result = (long*)malloc(sizeof(long) * iterations_);
  else
    result = (long*)malloc(sizeof(long));
  if (!result) {
    perror("Memory allocation failed");
    return;
  }

  StopwatchArgs stopwatchArgs{iterations_, calcIterations_, recordJitter_,
                              result};  //  all settins as a single object

  // preventing memory access conflicts
  if (mlockall(MCL_CURRENT | MCL_FUTURE)) {
    perror("mlockall");
    exit(EXIT_FAILURE);
  }

  pthread_create(&thread, attr_, loaderFunc,
                 &stopwatchArgs);  // running the experiment
  pthread_join(thread, NULL);      // waiting for the experiment to finish

  outputResult(result);

  free(result);
}

void ThreadStopwatch::outputResult(long* result) {
  double duration_ms = 0;  // overall duration of the experiment in ms
  // accumulate duration of every iteration if recordJitter_ is true, else get
  // overall duration in ms
  if (recordJitter_) {
    for (int i = 0; i < iterations_; i++) {
      duration_ms += result[i] / 1e6;  // ns to ms
    }
  } else
    duration_ms = *result / 1e6;  // ns to ms

  double delay = duration_ms - iterations_;  // overall delay
  double average_delay =
      delay / iterations_;  // avarage delay of every iteration

  printf(
      "Expected duration: %d ms \nReal duration: %f ms \nDelay: %f ms "
      "\nAvg "
      "delay: "
      "%f ms\n",
      iterations_, duration_ms, delay, average_delay);

  if (recordJitter_) saveResult(result);  // saving jitter measurements to txt
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
