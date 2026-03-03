#include <evl/clock.h>
#include <evl/thread.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

void* pi_loop(void* arg) {
  int iterations = *(int*)arg;

  struct timespec start, end, current;

  long* durations = malloc(sizeof(long) * iterations);
  if (!durations) return NULL;

  /* Attach this POSIX thread to EVL */
  if (evl_attach_self("rt-worker") < 0) {
    perror("evl_attach_self");
    return NULL;
  }

  evl_read_clock(EVL_CLOCK_MONOTONIC, &start);
  struct timespec next_time = start;

  for (int i = 0; i < iterations; i++) {
    next_time.tv_nsec += 1e6;

    if (next_time.tv_nsec >= 1e9) {
      next_time.tv_sec++;
      next_time.tv_nsec -= 1e9;
    }

    double pi = 2;
    for (int j = 2; j < 21; j += 2) {
      pi *= j * j / ((j - 1) * (j + 1));
    }

    evl_sleep_until(EVL_CLOCK_MONOTONIC, &next_time);
  }
  evl_read_clock(EVL_CLOCK_MONOTONIC, &end);

  long* duration = (long*)malloc(sizeof(long));
  *duration = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
  return duration;
}

int main() {
  pthread_t thread;
  int iterations = 10000;

  mlockall(MCL_CURRENT | MCL_FUTURE);

  pthread_create(&thread, NULL, pi_loop, &iterations);
  void* duration_ptr;
  pthread_join(thread, &duration_ptr);

  long duration_ns = *(long*)duration_ptr;
  double duration_ms = duration_ns / 1e6;
  double delay = duration_ms - iterations;
  double average_delay = delay / iterations;

  printf(
      "Expected duration: %d ms \nReal duration: %f ms \nDelay: %f ms \nAvg "
      "Delay: "
      "%f ms\n",
      iterations, duration_ms, delay, average_delay);

  return 0;
}