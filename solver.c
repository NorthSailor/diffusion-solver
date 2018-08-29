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

static void apply_boundary_conditions(struct Matrix *matrix,
				      struct BoundaryCondition *first)
{
	struct BoundaryCondition *b = first;
	while (b) {
		int stride = 1;
		int index = 0;

		/* The following code assumes that the matrices are zero-indexed
		 * and that the X value corresponds to columns while the Y value
		 * to rows. In other words: index = y * columns + x (row-major
		 * order) */
		switch (b->boundary) {
		case BC_XPLUS:
			index = matrix->columns * (matrix->rows - 1) + b->start;
			stride = 1;
			break;
		case BC_XMINUS:
			index = b->start;
			stride = 1;
			break;
		case BC_YMINUS:
			index = b->start * matrix->columns;
			stride = matrix->columns;
			break;
		case BC_YPLUS:
			index = (b->start + 1) * matrix->columns - 1;
			stride = matrix->columns;
			break;
		}

		for (int i = b->start; i < b->end; i++) {
			matrix->data[index] = b->value;
			index += stride;
		}

		b = b->next;
	}
}

static void solver_advance_iteration(struct Solver *solver)
{
	int stride = solver->problem->width;
	/* Apply the stencil while finding the residual for the previous
	 * iteration. */
	solver->residual = 0.0;
	number_t residual = 0.0;
#pragma omp parallel for reduction(+ : residual)
	for (int x = 1; x < (solver->problem->width - 1); x++) {
		for (int y = 1; y < (solver->problem->height - 1); y++) {
			int i = y * stride + x;
			number_t N = solver->source->data[i + stride];
			number_t S = solver->source->data[i - stride];
			number_t W = solver->source->data[i - 1];
			number_t E = solver->source->data[i + 1];
			number_t result = 0.25 * (N + S + W + E);
			solver->dest->data[i] = result;
			number_t center = solver->source->data[i];
			number_t point_error = center - result;
			residual += point_error * point_error;
		}
	}

	solver->residual = residual;

	/* Pointer swap. */
	struct Matrix *temp = solver->source;
	solver->source = solver->dest;
	solver->dest = temp;
}

void solver_solve(struct Solver *solver)
{
	printf("Solving...\n");

	apply_boundary_conditions(solver->source,
				  solver->problem->boundary_conditions);
	apply_boundary_conditions(solver->dest,
				  solver->problem->boundary_conditions);

	number_t tolerance = 0.005;
	int iteration_count = 0;

	while (1) {
		solver_advance_iteration(solver);
		iteration_count++;

		/* Check for convergence. */
		if (solver->residual < tolerance) {
			break;
		}

		if (iteration_count % 1000 == 0) {
			printf("Residual: %" NUMBER_FMT "\n", solver->residual);
		}
	}

	printf("Solution converged after %d iterations.\n", iteration_count);
}

void solver_save_results(struct Solver *solver, FILE *fp)
{
	matrix_write_to_file(solver->source, fp);
}
