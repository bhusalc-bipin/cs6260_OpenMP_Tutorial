/*
 * Bug Hunt Challenge -- SOLUTION
 *
 * DISCLAIMER: This file is created with the help of AI.
 *
 * This file contains the corrected version of bug_hunt.c with all 5 bugs identified and fixed.
 *
 * Compile:
 *  gcc -Wall -Wextra -fopenmp -o bug_hunt_solution bug_hunt_solution.c
 * OR
 *  make bug_hunt_solution
 * Run:     ./bug_hunt_solution <n> <thread_count>
 * Example: ./bug_hunt_solution 100 4
 *
 * NOTE: n must be >= 6 because the program accesses array[5] in the output.
 */

#include <limits.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <thread_count>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int n = atoi(argv[1]);
    unsigned int thread_count = atoi(argv[2]);

    if (n < 6) {
        printf("Please provide n >= 6.\n");
        return EXIT_FAILURE;
    }

    unsigned long* array = malloc(n * sizeof(unsigned long));

    /*
     * BUG 1 (Loop-carried dependency): array[i] depends on array[i-1], so each iteration needs the
     * result of the previous iteration. This is a loop-carried dependency and CANNOT be correctly
     * parallelized with a simple parallel for. Threads may read array[i-1] before another thread
     * has written it, producing garbage values.
     * Fix: Remove the parallel for and compute sequentially. This loop is inherently serial due to
     * the dependency. (Note: There are advanced techniques like parallel prefix sums that can
     * parallelize this pattern, but they are beyond the scope of this tutorial.)
     */
    array[0] = 1;
    for (unsigned int i = 1; i < n; i++) {
        array[i] = array[i - 1] + (i + 1);
    }

    unsigned long sum = 0;
    unsigned long min_val = ULONG_MAX;
    unsigned long max_val = 0;
    unsigned int even_count = 0;

    /*
     * BUG 2 (Race condition on sum): sum was updated by all threads simultaneously without any
     * protection, causing a race condition where updates could be lost.
     * Fix: reduction(+:sum)
     *
     * BUG 3 (Missing private variable): temp was declared outside the parallel region, making it
     * shared by default. All threads read and wrote to the same temp variable, corrupting each
     * other's values.
     * Fix: private(temp) -- or declare temp inside the loop body
     *
     * BUG 4 (Race condition on even_count): even_count was incremented by all threads without
     * protection, causing a race condition.
     * Fix: reduction(+:even_count)
     *
     * BUG 5 (Race condition on min_val and max_val): min_val and max_val were compared and updated
     * by all threads without protection.
     * Fix: reduction(min:min_val) reduction(max:max_val)
     */
    unsigned long temp;

#pragma omp parallel for num_threads(thread_count) private(temp) reduction(+ : sum, even_count)    \
    reduction(min : min_val) reduction(max : max_val)
    for (unsigned int i = 0; i < n; i++) {
        temp = array[i];

        sum += temp;

        if (temp < min_val) {
            min_val = temp;
        }

        if (temp > max_val) {
            max_val = temp;
        }

        if (temp % 2 == 0) {
            even_count++;
        }
    }

    double average = (double)sum / n;

    printf("Array[0]:   %lu\n", array[0]);
    printf("Array[1]:   %lu\n", array[1]);
    printf("Array[2]:   %lu\n", array[2]);
    printf("Array[3]:   %lu\n", array[3]);
    printf("Array[4]:   %lu\n", array[4]);
    printf("Array[5]:   %lu\n", array[5]);
    printf("...\n");
    printf("Array[%u]:  %lu\n", n - 1, array[n - 1]);
    printf("Sum:        %lu\n", sum);
    printf("Min:        %lu\n", min_val);
    printf("Max:        %lu\n", max_val);
    printf("Average:    %.2f\n", average);
    printf("Even count: %u\n", even_count);

    free(array);
    return EXIT_SUCCESS;
}