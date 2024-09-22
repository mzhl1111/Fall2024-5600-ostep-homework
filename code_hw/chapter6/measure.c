#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define ITERATIONS 1000000

int main() {
    struct timeval start, end;
    long elapsedTime;
    int i;

    gettimeofday(&start, NULL);

    for (i = 0; i < ITERATIONS; i++) {
        getpid();
    }

    gettimeofday(&end, NULL);

    elapsedTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    double avgSysCallTime = (double)elapsedTime / ITERATIONS;
    printf("Average system call time: %lf ms\n", avgSysCallTime);

    return 0;
}

// Average system call time: 0.707683 ms