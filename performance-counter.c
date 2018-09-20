#include "performance-counter.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

struct PerformanceCounter {
	struct timeval starttime;
	struct timeval endtime;
	int iteration_count;
};

struct PerformanceCounter *performance_counter_new()
{
	return malloc(sizeof(struct PerformanceCounter));
}

void performance_counter_free(struct PerformanceCounter *pf)
{
	free(pf);
}

void performance_counter_start(struct PerformanceCounter *pf)
{
	gettimeofday(&pf->starttime, NULL);
}

void performance_counter_stop(struct PerformanceCounter *pf)
{
	gettimeofday(&pf->endtime, NULL);
}

void performance_counter_set_iteration_count(struct PerformanceCounter *pf,
					     int iteration_count)
{
	pf->iteration_count = iteration_count;
}

void performance_counter_print_statistics(struct PerformanceCounter *pf)
{
	// Calculate the total number of microseconds.
	long ms_elapsed = 1000000 * (pf->endtime.tv_sec - pf->starttime.tv_sec)
			  + (pf->endtime.tv_usec - pf->starttime.tv_usec);
	double iterations_per_second =
		(double)pf->iteration_count / (double)ms_elapsed * 1000000;
	printf("---------------------------------------------------\n");
	printf("Iterations: %d\n", pf->iteration_count);
	printf("Elapsed time: %.2g s\n", ms_elapsed * 0.000001);
	printf("%g iterations/s\n", iterations_per_second);
	printf("%g ms/iteration\n", 1000 / iterations_per_second);
	printf("---------------------------------------------------\n");
}
