/*
 * This program teaches about the "schedule" clause in OpenMP, which allows you to specify how
 * iterations of a parallel loop are divided among threads.
 *
 * Syntax of Schedule Clause: schedule(type, [chunk_size])
 *
 * where, type can be one of the following:
 *
 * 1. Static Scheduling:
 *      The iterations are divided into equal-sized chunks and assigned to threads in a
 * round-robin fashion. With static scheduling, iteratons are assigned to the threads before the
 * loop execution begins. Static scheduling is good for loops where the workload is evenly
 * distributed across iterations. If no chunk size is provided, it defaults to total_iterations /
 * number_of_threads.
 *
 * 2. Dynamic Scheduling:
 *     Initially, each thread is assigned a chunk of iteration(s). When a thread finishes its chunk,
 * it requests the next chunk of iterations until all iterations are completed. This is useful for
 * loops where the workload is unevenly distributed across iterations, as it allows for better load
 * balancing. When chunk_size is not specified, it defaults to 1.
 *
 * 3. Guided Scheduling:
 *     Similar to dynamic scheduling, but the chunk size starts large and decreases as threads
 * request more work. This helps with load balancing across threads when later iterations are more
 * compute intensive. When chunk_size is not specified, chunk size decreases to 1. If chunk_size is
 * specified, it decreases to chunk_size (very last chunk can be smaller than chunk_size).
 *
 * 4. Auto Scheduling:
 *     The decision on how to schedule the iterations is left to the compiler and runtime system. No
 * chunk size can be specified with auto scheduling.
 *
 * 5. Runtime Scheduling:
 *     The scheduling type and chunk size both are determined at runtime using OMP_SCHEDULE
 * environment variable.
 *
 * NOTE: In terms of overhead, overhead(no schedule) < overhead(static) < overhead(dynamic) <
 * overhead(guided) because static scheduling has no runtime overhead, while dynamic and guided
 * scheduling require runtime management of work distribution. The choice of scheduling type and
 * chunk size can impact the performance of a parallel loop, and it may require experimentation to
 * find the best configuration.
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <upper_bound>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int thread_count = atoi(argv[1]);
    unsigned long upper_bound = atol(argv[2]);

    unsigned long global_sum = 0;

#pragma omp parallel for num_threads(thread_count) reduction(+ : global_sum) schedule(static, 2)
    for (unsigned long i = 1; i <= upper_bound; i++) {
        global_sum += i;
    }

    // Expected sum from 1 to n is (n*(n+1))/2, so using this formula to verify our result.
    unsigned long expected_sum = (upper_bound * (upper_bound + 1)) / 2;
    printf("Expected sum from 1 to %lu: %lu\n", upper_bound, expected_sum);
    printf("Sum we computed from 1 to %lu: %lu\n", upper_bound, global_sum);
    printf("Result is %s\n", (global_sum == expected_sum) ? "correct!!!" : "incorrect!");

    return EXIT_SUCCESS;
}