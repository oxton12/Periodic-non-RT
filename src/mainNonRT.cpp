#include <LoaderNonRT.h>

#include <ThreadStopwatch.hpp>

int main() {
  ThreadStopwatch stopwatch = ThreadStopwatch(10000, 150000, true, NULL);
  stopwatch.runTest(loaderNonRT);
}