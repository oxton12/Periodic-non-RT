//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : An object to store all the experiment settings and the result
//==============================================================================

#ifndef __THREADSTOPWATCHARGS_H__
#define __THREADSTOPWATCHARGS_H__

/// @brief An object to store all the experiment settings and the result
/// @param iterations how many times the function is executed
/// @param calcIterations amount of iterations done inside the executed function
/// @param recordJitter whether to record jitter or an overall delay
/// @param result pointer to experiment results
typedef struct {
  int iterations;
  int calcIterations;
  bool recordJitter;
  long* result;
} StopwatchArgs;
#endif  // __THREADSTOPWATCHARGS_H__