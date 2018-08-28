#include "solver.h"
#include "matrix.h"
#include "problem.h"
#include <stdlib.h>

struct Solver *solver_from_problem(struct Problem *problem)
{
	struct Solver *solver = (struct Solver *)malloc(sizeof(*solver));
	solver->problem = problem;
	solver->source = matrix_new(problem->width, problem->height);
	solver->dest = matrix_new(problem->width, problem->height);
	solver->residual = 10000.0;
	return solver;
}

void solver_free(struct Solver *solver)
{
	problem_free(solver->problem);
	matrix_free(solver->source);
	matrix_free(solver->dest);
	free(solver);
}

static void solver_advance_iteration(struct Solver *solver)
{
	solver->residual -= 0.00001;
}

void solver_solve(struct Solver *solver)
{
	printf("Solving...\n");

	number_t tolerance = 0.05;
	int iteration_count = 0;

	while (1) {
		solver_advance_iteration(solver);
		iteration_count++;

		/* Check for convergence. */
		if (solver->residual < tolerance) {
			break;
		}
	}

	printf("Solution converged after %d iterations.\n", iteration_count);
}
