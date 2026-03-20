//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : Running the non-real-time experiment
//==============================================================================

#include <LoaderNonRT.h>

#include <ThreadStopwatch.hpp>

int main() {
  int iterations = 10000;  // how many times the function is executed
  int calcIterations =
      150000;  // amount of iterations done inside the executed function
  bool recordJitter = false;  // whether to record jitter or an overall delay

  ThreadStopwatch stopwatch =
      ThreadStopwatch(iterations, calcIterations, recordJitter, NULL);
  stopwatch.runTest(loaderNonRT);  // run the experiment
}