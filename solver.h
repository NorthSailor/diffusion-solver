#ifndef SOLVER_H
#define SOLVER_H

#include "number.h"
#include <stdio.h>

struct Matrix;

struct Solver {
	struct Problem *problem;

	struct Matrix *source;
	struct Matrix *dest;

	number_t residual;

	int iteration_count;
};

struct Solver *solver_from_problem(struct Problem *problem);
void solver_free(struct Solver *solver);

void solver_solve(struct Solver *solver);

void solver_save_results(struct Solver *solver, FILE *fp);
void solver_save_velocity(struct Solver *solver, FILE *fp);

int solver_get_iteration_count(struct Solver *solver);

#endif /* SOLVER_H */
