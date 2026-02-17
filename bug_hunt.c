/*
 * Bug Hunt Challenge
 *
 * DISCLAIMER: This file is created with the help of AI.
 *
 * This program is supposed to:
 *   1. Fill an array where each element is the cumulative sum: array[i] = array[i-1] + (i + 1)
 *      This means: array[0] = 1, array[1] = 3, array[2] = 6, array[3] = 10, ...
 *      (These are called triangular numbers)
 *   2. Compute the sum of all elements
 *   3. Find the minimum value
 *   4. Find the maximum value
 *   5. Compute the average
 *   6. Count how many elements are even
 *
 * Expected output for N = 100, 4 threads:
 *   Array[0]:   1
 *   Array[1]:   3
 *   Array[2]:   6
 *   Array[3]:   10
 *   Array[4]:   15
 *   Array[5]:   21
 *   ...
 *   Array[99]:  5050
 *   Sum:        171700
 *   Min:        1
 *   Max:        5050
 *   Average:    1717.00
 *   Even count: 50
 *
 * However, this program has MULTIPLE bugs related to OpenMP and parallelization concepts. Your task
 * is to find and fix ALL of them.
 *
 * Notes:
 *   - There are 5 bugs in this program.
 *   - All bugs are related to OpenMP parallelization concepts.
 *   - The serial logic of the program is correct.
 *   - Run the program multiple times; some bugs may cause inconsistent results across runs.
 *
 * Compile:
 *  gcc -g -Wall -Wextra -fopenmp -o bug_hunt bug_hunt.c
 * OR
 *  make bug_hunt
 * Run:     ./bug_hunt <n> <thread_count>
 * Example: ./bug_hunt 100 4
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

    array[0] = 1;
#pragma omp parallel for num_threads(thread_count) schedule(static, 1)
    for (unsigned int i = 1; i < n; i++) {
        array[i] = array[i - 1] + (i + 1);
    }

    unsigned long sum = 0;
    unsigned long min_val = ULONG_MAX;
    unsigned long max_val = 0;
    unsigned int even_count = 0;

    unsigned long temp;

#pragma omp parallel for num_threads(thread_count)
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