# CS6260 -- OpenMP Tutorial

This tutorial introduces you to OpenMP.

**NOTE:** This is an introductory tutorial, not an in-depth coverage. For in-depth information, please refer to the official OpenMP website and specification.

## Resources

- [OpenMP Official Website](https://www.openmp.org/)
- [OpenMP Specification](https://www.openmp.org/specifications/)
- [An Introduction to Parallel Programming 2nd Edition by Peter Pacheco and Matthew Malensek](https://www.cs.usfca.edu/~peter/ipp2/index.html)

## Tutorial Order

1. intro.c
2. scope.c
3. reduction.c
4. parallel_for.c
5. scheduling.c

## Program

To keep programs small and simple, all examples except intro.c use a program that calculates the sum of 1 through a user-specified number using a user-specified number of threads.

## Most Common OpenMP Directives

### Covered in This Tutorial

1. **parallel**: Specify a parallel region
2. **parallel for**: Parallelize a for loop
3. **critical**: Specify a region which should be executed by threads one at a time (handles race conditions)
4. **barrier**: Synchronize all threads

### Not Covered in This Tutorial

5. **atomic**: Lightweight alternative to critical but has limitations compared to critical
6. **master**: Specify that only the main thread should execute a section
7. **single**: Specify a section that should be executed by only one thread, whichever thread reaches it first

**NOTE:** There are many other helpful directives as well. You can explore those in the OpenMP official documentation.

## Disclaimer

1. To keep programs simple and focused on OpenMP concepts, I am not doing extensive error checking. Also, the approach I am using in this tutorial might not be the optimal approach.
2. I have used AI (autocomplete feature) to help me write the comments (code descriptions and concept explanations) whenever it was correct.
