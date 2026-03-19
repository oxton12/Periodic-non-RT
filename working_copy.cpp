#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <evl/thread.h>
#include <evl/clock.h>
#include <evl/timer.h>
#include <evl/sched.h>
#include <evl/mutex.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sched.h>
#include <pthread.h>

#define PERIOD_NS 1000000
#define RUN_TIME 5
#define CPU_CORE 1
#define FILE_NAME "ass22.csv"

static struct evl_mutex rt_mutex; // mutex struct

// store execution times (in miliseconds)
static double exec_times[RUN_TIME * 1000];

void *real_time_task(void *arg) {

    struct timespec next_time, start_time, end_time; // timing vars

    clock_gettime(CLOCK_MONOTONIC, &next_time); // get time

    // attach thread to evl
    if (evl_attach_self("rt-task") < 0) {
        perror("attach failed");
        return NULL;
    }

    printf("rt task started on cpu %d\n", CPU_CORE);

    for (int i = 0; i < (RUN_TIME * 1000); i++) {

        evl_read_clock(CLOCK_MONOTONIC, &start_time); // start

        // small load
        volatile int product = 0;
        for (int j = 0; j < 100000; j++) product += j*j;

        // update next time
        next_time.tv_nsec += PERIOD_NS;
        while (next_time.tv_nsec >= 1000000000) {
            next_time.tv_sec++;
            next_time.tv_nsec -= 1000000000;
        }

        evl_sleep_until(EVL_CLOCK_MONOTONIC, &next_time); // sleep

	evl_read_clock(CLOCK_MONOTONIC, &end_time); // end

        // calc time
        exec_times[i] = (end_time.tv_sec - start_time.tv_sec) * 1e9 +
                        (end_time.tv_nsec - start_time.tv_nsec);

    }
    return NULL;
}

int main() {

    int ret;
    pthread_t rt_thread;
    struct evl_sched_attrs attr;
    cpu_set_t cpu_set;

    // scheduler config
    attr.sched_policy = SCHED_FIFO;
    attr.sched_priority = 80;

    // attach main thread


    // cpu affinity
    CPU_ZERO(&cpu_set);
    CPU_SET(CPU_CORE, &cpu_set);

    pthread_attr_t attr_thread;
    pthread_attr_init(&attr_thread);
    pthread_attr_setaffinity_np(&attr_thread, sizeof(cpu_set_t), &cpu_set);

    printf("starting task on cpu %d\n", CPU_CORE);

    // create thread
    pthread_create(&rt_thread, &attr_thread, real_time_task, NULL);
    pthread_join(rt_thread, NULL);

    // open file
    int log_fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd < 0) {
        perror("file open fail");
        return -1;
    }

    // write results
    char log_buffer[256];
    for (int i = 0; i < (RUN_TIME * 1000); i++) {
        snprintf(log_buffer, sizeof(log_buffer), "Iteration %d - Execution Time: %.3f ns\n", i, exec_times[i]);
        write(log_fd, log_buffer, strlen(log_buffer));
    }

    close(log_fd); // close file

    return 0;
}
