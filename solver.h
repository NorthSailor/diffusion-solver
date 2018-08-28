#ifndef SOLVER_H
#define SOLVER_H

#include "number.h"

struct Matrix;

struct Solver {
	struct Problem *problem;

	struct Matrix *source;
	struct Matrix *dest;

	number_t residual;
};

struct Solver *solver_from_problem(struct Problem *problem);
void solver_free(struct Solver *solver);

void solver_solve(struct Solver *solver);

#endif /* SOLVER_H */
