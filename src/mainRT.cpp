#include <LoaderRT.h>
#include <evl/sched.h>

#include <ThreadStopwatch.hpp>

int main() {
    pthread_t rt_thread;
    struct evl_sched_attrs attr;
    cpu_set_t cpu_set;

    // scheduler config
    attr.sched_policy = SCHED_FIFO;
    attr.sched_priority = 80;

    // attach main thread


    // cpu affinity
    CPU_ZERO(&cpu_set);
    CPU_SET(1, &cpu_set);

    pthread_attr_t attr_thread;
    pthread_attr_init(&attr_thread);
    pthread_attr_setaffinity_np(&attr_thread, sizeof(cpu_set_t), &cpu_set);

  ThreadStopwatch stopwatch = ThreadStopwatch(10000, 2000, false, &attr);
  stopwatch.runTest(loaderRT);
}