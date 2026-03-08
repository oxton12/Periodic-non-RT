#include <LoaderRT.h>

#include <ThreadStopwatch.hpp>

int main() {
  ThreadStopwatch stopwatch = ThreadStopwatch(10000, 1000, false);
  stopwatch.runTest(loaderRT);
}