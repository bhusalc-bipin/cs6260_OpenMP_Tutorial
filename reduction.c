/*
 * This program teaches about reduction in OpenMP.
 *
 * Reduction in OpenMP:
 *
 * The reduction clause is used to combine values from multiple threads into a single value. It is
 * useful when we want to compute a single value (like sum, max, min) from values computed by
 * multiple threads. The reduction clause automatically handles the synchronization and combining of
 * values from different threads.
 *
 * Syntax: reduction(OPERATOR:REDUCTION_VARIABLES)
 *
 * where,
 * OPERATOR is one of: +, -, *, &, |, ^, &&, ||, max, min
 * REDUCTION_VARIABLES are the variables that are being reduced.
 *
 * NOTE: The reduction clause creates a private copy of the reduction variable for each thread and
 * initializes it to the identity value of the operator (e.g., 0 for +, 1 for *, etc.). At the end
 * of the parallel region, the private copies are combined using the specified operator and the
 * result is stored in the original variable.
 *
 * Caution: floating-point reduction can lead to non-deterministic results due to the
 * non-associative nature of floating-point arithmetic.
 *
 * NOTE: Eventhough subtraction is not associative, it is still allowed in reduction clause because
 * OpenMP internally converts it to addition of partial results with appropriate sign changes.
 * For example: a - b - c can be expressed as a + (-b) + (-c) to make subtraction associative.
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

#pragma omp parallel num_threads(thread_count) reduction(+ : global_sum)
    {
        unsigned int tid = omp_get_thread_num();

        // Calculate bounds for each thread
        unsigned long local_start = (upper_bound * tid) / thread_count + 1;
        unsigned long local_end = (upper_bound * (tid + 1)) / thread_count;

        printf("Thread %u: local_start = %lu, local_end = %lu\n", tid, local_start, local_end);

        for (unsigned long i = local_start; i <= local_end; i++) {
            // NOTE: This global_sum is private to each thread due to the reduction clause. The
            // reduction clause will take care of combining these private copies into a single
            // shared global_sum at the end of the parallel region.
            global_sum += i;
        }
    }

    printf("\n");

    // Expected sum from 1 to n is (n*(n+1))/2, so using this formula to verify our result.
    unsigned long expected_sum = (upper_bound * (upper_bound + 1)) / 2;
    printf("Expected sum from 1 to %lu: %lu\n", upper_bound, expected_sum);
    printf("Sum we computed from 1 to %lu: %lu\n", upper_bound, global_sum);
    printf("Result is %s\n", (global_sum == expected_sum) ? "correct!!!" : "incorrect!");

    return EXIT_SUCCESS;
}
