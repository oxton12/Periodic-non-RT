//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A class used to prepare for measuring the delay (or jitter) of a
// peridically executed function, run an experiment, output and save the
// results. Can be used for both real-time and non-real-time experiments.
//==============================================================================

#ifndef __THREADSTOPWATCH_H__
#define __THREADSTOPWATCH_H__

#include <ThreadStopwatchArgs.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

class ThreadStopwatch {
 public:
  /// @brief ThreadStopwatch constructor
  /// @param iterations how many times the function is executed
  /// @param calcIterations amount of iterations done inside the
  // executed function
  /// @param recordJitter whether to record jitter or an overall delay
  /// @param attr a pointer to a thread attributes object that defines thread
  /// properties
  ThreadStopwatch(int iterations, int calcIterations, bool recordJitter,
                  const pthread_attr_t* attr);

  /// @brief Runs the experiment with the provided function.
  /// @param loaderFunc Function that measures the delay (jitter). Can be either
  /// real-time or non-real-time
  void runTest(void* (*loaderFunc)(void*));

 private:
  int iterations_;      // how many times the function is executed
  int calcIterations_;  // mount of iterations done inside the executed function
  bool recordJitter_;   // whether to record jitter or an overall delay
  const pthread_attr_t* attr_;  // a pointer to a thread attributes object that
                                // defines thread properties

  void outputResult(
      long* result);  // prints the experiment results in the console
  void saveResult(long* result);  // saves the recorded jitter in a .txt file
};

#endif  // __THREADSTOPWATCH_H__