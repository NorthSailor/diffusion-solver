#ifndef PERFORMANCE_COUNTER_H
#define PERFORMANCE_COUNTER_H

struct PerformanceCounter;

struct PerformanceCounter *performance_counter_new();
void performance_counter_free(struct PerformanceCounter *pf);

void performance_counter_start(struct PerformanceCounter *pf);
void performance_counter_stop(struct PerformanceCounter *pf);

void performance_counter_set_iteration_count(struct PerformanceCounter *pf,
					     int iteration_count);

void performance_counter_print_statistics(struct PerformanceCounter *pf);

#endif /* PERFORMANCE_COUNTER_H */
