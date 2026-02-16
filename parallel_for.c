/*
 * This program teaches about the "parallel for" directive in OpenMP.
 *
 * Parallel For Directive:
 *
 * The "parallel for" directive in OpenMP is used to parallelize loops. It automatically divides the
 * loop iterations among the threads in a parallel region. If we don't specify partition strategy
 * (scheduling), then by default it assigns contiguous chunks of size (number of iterations / number
 * of threads) to each thread. We can also specify partition strategy using "schedule" clause, which
 * is covered in omp_schedule.c.
 *
 * Next instruction after the "parallel for" directive must be a for loop in canonical form. The
 * canonical form allows the iteration count of loops to be computed before executing the loop. Few
 * common examples of not being in canonical form include:
 *      - Using while loops or do-while loops instead of for loops.
 *      - Using break inside the loop body
 *      - Using for loop as forever loop (e.g., for(;;))
 *      - Body of for loop not being a structured block.
 *      - Modifying the loop variable inside the loop body.
 *
 * Key idea here is that the number of iterations of the loop must be computable before executing
 * the loop.
 *
 * NOTE: "parallel for" doesn't work with while loops, do-while loops, or for loops that are not in
 * the canonical form. Follow the link below for more details about canonical form of for loops:
 * https://www.openmp.org/spec-html/5.0/openmpsu40.html
 *
 * The loop variable in the for loop is private to each thread by default.
 *
 * CAUTION: Be careful of loop-carried dependencies when using "parallel for". If there are
 * dependencies between iterations of the loop, it can lead to incorrect results. In such cases, we
 * need to remove the dependencies or consider using a different parallelization strategy.
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

#pragma omp parallel for num_threads(thread_count) reduction(+ : global_sum)
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
