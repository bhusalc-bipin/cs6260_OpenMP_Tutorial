/*
 * This program teaches about scope of variables in OpenMP and how to handle race conditons.
 *
 * Variable Scope in OpenMP:
 *
 * In OpenMP there are two types of variable scopes: shared and private. Shared variables are
 * accessible by all threads in a parallel region, while private variables are local to each thread
 * and cannot be accessed by other threads. By default, all variables declared outside the parallel
 * region are shared, and all variables declared inside the parallel region are private. However, we
 * can explicitly specify the scope of variables using "shared" and "private" clauses.
 *
 * We can also use "default(none)" clause to force ourselves to explicitly specify the scope of all
 * variables used in the parallel region but are declared outside the parallel region.
 *
 *
 * Critical Section:
 *
 * A critical section is a part of the code that must be executed by only one thread at a time to
 * avoid race conditions. In OpenMP, we can use the "critical" directive to define a critical
 * section. This ensures that only one thread can access the shared resource at a time, preventing
 * race conditions.
 *
 * NOTE: There are also other ways to handle race condition in OpenMP. For example: atomic
 * operations, locks. For this tutorial, I am using the critical directive.
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
    unsigned long local_sum;

    // NOTE: Here if we don't use default(none) then we don't need to specify "shared(global_sum,
    // upper_bound)" because by default all variables declared outside the parallel region are
    // shared. I am just showing it here for demonstration purpose. Similarly, we don't need to
    // specify "private(local_sum)" this way. Instead we could have declared local_sum inside the
    // parallel region because by default all variables declared inside the parallel region are
    // private. But I am showing it here for demonstration purpose to show how to specify variable
    // scope explicitly using OpenMP clauses.
#pragma omp parallel num_threads(thread_count) default(none) private(local_sum)                    \
    shared(global_sum, upper_bound)
    {
        // Get the thread ID of the current thread
        unsigned int tid = omp_get_thread_num();
        // Get the total number of threads. We already have this information in "thread_count", but
        // I am showing how to get it using OpenMP function for demonstration purpose.
        unsigned int num_threads = omp_get_num_threads();

        // Calculate bounds for each thread
        unsigned long local_start = (upper_bound * tid) / num_threads + 1;
        unsigned long local_end = (upper_bound * (tid + 1)) / num_threads;

        printf("Thread %u: local_start = %lu, local_end = %lu\n", tid, local_start, local_end);

        local_sum = 0;
        for (unsigned long i = local_start; i <= local_end; i++) {
            local_sum += i;
        }

        // Now we need to add local_sum to the global_sum. This is where race condition occur if
        // multiple threads try to update global_sum at the same time. To avoid this, we can use a
        // critical directive to ensure that only one thread updates global_sum at a time.
#pragma omp critical
        {
            global_sum += local_sum;
        }
    }

    printf("\n"); // Just to add a newline after the thread outputs

    // Expected sum from 1 to n is (n*(n+1))/2, so using this formula to verify our result.
    unsigned long expected_sum = (upper_bound * (upper_bound + 1)) / 2;
    printf("Expected sum from 1 to %lu: %lu\n", upper_bound, expected_sum);
    printf("Sum we computed from 1 to %lu: %lu\n", upper_bound, global_sum);
    printf("Result is %s\n", (global_sum == expected_sum) ? "correct!!!" : "incorrect!");

    return EXIT_SUCCESS;
}