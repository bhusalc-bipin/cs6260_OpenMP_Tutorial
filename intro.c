/*
 * Introduction to OpenMP (Open Multi-Processing)
 *
 * OpenMP is an API that supports SHARED-MEMORY parallel programming in C, C++ and Fortran. We can
 * use directives provided by OpenMP API to parallelize our code. The directives are implemented as
 * pragmas in C/C++.
 *
 * OpenMP uses fork-join model of parallel execution. The program starts with a single thread
 * (called the master thread, thread with thread ID 0). When the program encounters a parallel
 * region (as specified by the OpenMP directives), it creates a team of threads. Each thread
 * executes the code within the parallel region. After the parallel region, all the threads join
 * back to the master thread and continue executing sequentially. This process continues until the
 * end of the program.
 *
 * To use OpenMP in C/C++, we need to include the "omp.h" header file. Then we can use the OpenMP
 * directives to parallelize our code. OpenMP directives in C/C++ have the following format:
 *
 * #pragma omp <directive-name> [clause1] [clause2] ...
 *
 * Here, clause1, clause2, etc. are optional and there can be only one directive-name per directive.
 *
 * Each directive applies to the structured block of code (one entry, one exit) or statement that
 * follows it. Long directives can be split into multiple lines using the backslash character (\) at
 * the end of each line.
 *
 * To compile a program with OpenMP directives, we need to use the "-fopenmp" flag with the
 * compiler. For example, to compile this program, we can use the following command:
 *
 * gcc -fopenmp -o intro intro.c
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned int thread_count
        = atoi(argv[1]); // Get the number of threads from command line argument

    // If num_threads is not specified, it will use the number of available cores as default. We can
    // also set the number of threads using the OMP_NUM_THREADS environment variable.
#pragma omp parallel num_threads(thread_count)
    {
        // Each thread has its own private stack so it can have its own local/private variables.
        // Like "tid" in this case.
        unsigned int tid = omp_get_thread_num(); // Get the thread ID of the current thread

        printf("Hello from thread %u\n", tid);

        // #pragma omp barrier // Synchronize all threads at this point
        //         if (tid == 0) {
        //             printf("Hello again from the master thread (thread %u)\n", tid);
        //         }
    }
    // Parallel region ends here. There is an implicit barrier at the end of the parallel region, so
    // all threads will wait for each other before proceeding.

    return EXIT_SUCCESS;
}
