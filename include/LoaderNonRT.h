//==============================================================================
// Authors : Max Solis, Aleksei Obshatko
// Group : ASDfR 5
// License : LGPL open source license
//
// Brief : A function that measures a delay in execution of a repeating task
// executed every 1 ms in a non-real-time fashion
//==============================================================================

#ifndef __LOADERNONRT_H__
#define __LOADERNONRT_H__

#include <Incrementinator.h>
#include <ThreadStopwatchArgs.h>
#include <pthread.h>
#include <stdlib.h>

/// @brief executes a function every 1 ms in a non-real-time fashion and
/// measures the delay. Only use as an argument for pthread_create
/// @param arg a pointer to StopwatchArgs object describing the paramters and
/// storing a pointer to the meassurements result
void* loaderNonRT(void* arg);

#endif  // __LOADERNONRT_H__