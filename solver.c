#include "solver.h"
#include "matrix.h"
#include "problem.h"
#include "math.h"
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

static void apply_boundary_condition(struct Matrix *matrix,
				     struct BoundaryCondition *b)
{
	int stride = 1;
	int index = 0;
	int derivative_offset = 0;

	/* The following code assumes that the matrices are zero-indexed
	 * and that the X value corresponds to columns while the Y value
	 * to rows. In other words: index = y * columns + x (row-major
	 * order) */
	switch (b->boundary) {
	case BC_XPLUS:
		index = matrix->columns * (matrix->rows - 1) + b->start;
		stride = 1;
		derivative_offset = -matrix->columns;
		break;
	case BC_XMINUS:
		index = b->start;
		stride = 1;
		derivative_offset = matrix->columns;
		break;
	case BC_YMINUS:
		index = b->start * matrix->columns;
		stride = matrix->columns;
		derivative_offset = 1;
		break;
	case BC_YPLUS:
		index = (b->start + 1) * matrix->columns - 1;
		stride = matrix->columns;
		derivative_offset = -1;
		break;
	}

	for (int i = b->start; i < b->end; i++) {
		float value;
		switch (b->type) {
		case BC_DIRICHLET:
			value = b->value;
			break;
		case BC_NEUMANN:
			value = matrix->data[index + derivative_offset]
				+ b->value;
		}
		matrix->data[index] = value;
		index += stride;
	}
}

static void apply_boundary_conditions(struct Matrix *matrix,
				      struct BoundaryCondition *first)
{
	/* Apply no-flow conditions everywhere that may be overwritten by fixed
	 * potential values. */
	/* X */
	int plus_offset_x = matrix->columns * (matrix->rows - 1);
	for (int i = 0; i < matrix->columns; i++) {
		matrix->data[i] = matrix->data[i + matrix->columns];
		matrix->data[i + plus_offset_x] =
			matrix->data[i + plus_offset_x - matrix->columns];
	}
	/* Y */
	int plus_offset_y = matrix->columns - 1;
	int stride_y = matrix->columns;
	for (int i = 0; i < matrix->rows; i++) {
		matrix->data[i * matrix->columns] =
			matrix->data[i * matrix->columns + 1];
		matrix->data[i * matrix->columns + plus_offset_y] =
			matrix->data[i * matrix->columns + plus_offset_y - 1];
	}

	struct BoundaryCondition *b = first;
	while (b) {
		apply_boundary_condition(matrix, b);
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

	number_t tolerance = 0.000005;
	int iteration_count = 0;
	int max_iterations = 5000000; /* @todo Let the user adjust this. */

	while (1) {
		solver_advance_iteration(solver);
		apply_boundary_conditions(solver->source,
					  solver->problem->boundary_conditions);
		iteration_count++;

		/* Check for convergence. */
		if (solver->residual < tolerance) {
			printf("Solution converged.\n");
			break;
		}

		if (iteration_count % 1000 == 0) {
			printf("Iteration: %d - Residual: %f\n",
			       iteration_count, solver->residual);
		}

		if (iteration_count > max_iterations) {
			printf("Maximum iteration count exceeded.\n");
			break;
		}
	}
	solver->iteration_count = iteration_count;
}

void solver_save_results(struct Solver *solver, FILE *fp)
{
	matrix_write_to_file(solver->source, fp);
}

void solver_save_velocity(struct Solver *solver, FILE *fp)
{
	struct Matrix *psi = solver->source;
	struct Matrix *u = matrix_new(psi->rows, psi->columns);
	struct Matrix *v = matrix_new(psi->rows, psi->columns);

	for (int x = 0; x < psi->columns; x++) {
		for (int y = 0; y < psi->rows; y++) {
			int index_N = (y + 1) * psi->columns + x;
			int index_S = (y - 1) * psi->columns + x;
			int index_W = y * psi->columns + x - 1;
			int index_E = y * psi->columns + x + 1;
			int index = y * psi->columns + x;

			/* U field */
			float uval = 0.0f;
			if (x == 0) {
				uval = psi->data[index_E] - psi->data[index];
			} else if (x == (psi->columns - 1)) {
				uval = psi->data[index] - psi->data[index_W];
			} else {
				uval = 0.5f
				       * (psi->data[index_E]
					  - psi->data[index_W]);
			}

			/* V field */
			float vval = 0.0f;
			if (y == 0) {
				vval = psi->data[index_N] - psi->data[index];
			} else if (y == (psi->rows - 1)) {
				vval = psi->data[index] - psi->data[index_S];
			} else {
				vval = 0.5f
				       * (psi->data[index_N]
					  - psi->data[index_S]);
			}
			u->data[index] = uval;
			v->data[index] = vval;
		}
	}

	matrix_write_vector_field(u, v, fp);

	matrix_free(u);
	matrix_free(v);
}

int solver_get_iteration_count(struct Solver *solver)
{
	return solver->iteration_count;
}
