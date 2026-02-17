CC = gcc
CFLAGS = -fopenmp -Wall -Wextra -O2

.PHONY: all intro scope reduction parallel_for scheduling bug_hunt bug_hunt_solution clean

all: intro scope reduction parallel_for scheduling bug_hunt bug_hunt_solution

intro: intro.c
	$(CC) $(CFLAGS) -o intro intro.c

scope: scope.c
	$(CC) $(CFLAGS) -o scope scope.c

reduction: reduction.c
	$(CC) $(CFLAGS) -o reduction reduction.c

parallel_for: parallel_for.c
	$(CC) $(CFLAGS) -o parallel_for parallel_for.c

scheduling: scheduling.c
	$(CC) $(CFLAGS) -o scheduling scheduling.c

bug_hunt: bug_hunt.c
	$(CC) $(CFLAGS) -o bug_hunt bug_hunt.c

bug_hunt_solution: bug_hunt_solution.c
	$(CC) $(CFLAGS) -o bug_hunt_solution bug_hunt_solution.c

clean:
	rm -f intro scope reduction parallel_for scheduling bug_hunt bug_hunt_solution
