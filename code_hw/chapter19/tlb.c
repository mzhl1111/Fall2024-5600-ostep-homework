// refered and corrected the code from:https://github.com/arush15june/OSTEP-Solutions/blob/master/Chapter%2019%20-%20TLB/tlb.c

#define _GNU_SOURCE 
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>

long double run_trial(const int numpages, uint64_t trials) {
    int PAGESIZE = getpagesize(); 
    size_t jump = PAGESIZE / sizeof(int);
    struct timespec start_time;
    struct timespec end_time;


    size_t num_ints = numpages * jump;
    int *arr = (int*)calloc(num_ints, sizeof(int));
    if (arr == NULL) {
        exit(1);
    }

    uint64_t initial_trials = trials;


    clock_gettime(CLOCK_MONOTONIC, &start_time);
    while(trials > 0) {
        for(size_t page = 0; page < numpages; page++) {
            arr[page * jump] += 1;
        }
        trials -= 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &end_time);


    long double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e9; 
    time_taken += (end_time.tv_nsec - start_time.tv_nsec); 
    
    printf("Total time: %Lf ns\n", time_taken);


    long double total_accesses = (long double)(numpages * initial_trials);
    long double average_time = time_taken / total_accesses;

    free(arr);

    return average_time;
}

int main(int argc, char** argv) {
    if(argc != 3) {
        puts("Usage: ./tlb <number of pages> <number of trials>");
        exit(EXIT_FAILURE);
    }


    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset)) {
        perror("Error setting CPU affinity");
        exit(EXIT_FAILURE);
    }

    size_t NUMPAGES = atoi(argv[1]);
    size_t TRIALS = atoi(argv[2]);


    struct timespec res_time;
    clock_getres(CLOCK_MONOTONIC, &res_time);

    long double ACCESS_TIME = run_trial(NUMPAGES, TRIALS);
    printf("Average access time per page: %Lf ns\n", ACCESS_TIME);

    return 0;
}
// run with 100, 100
// Total time: 225997.000000 ns
// Average access time per page: 22.599700 ns
