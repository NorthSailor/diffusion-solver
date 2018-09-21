#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "matrix.h"
#include "performance-counter.h"
#include "problem.h"
#include "solver.h"
#include "xdmf-generator.h"

void print_double_usage()
{
#ifdef USE_DOUBLES
	printf("Double precision: Yes\n");
#else
	printf("Double precision: No\n");
#endif
}

void print_usage()
{
	printf("Usage:\n\tdiffusion INPUT-FILE\n");
}

static void save_results(struct Solver *solver)
{
	FILE *output_fp = fopen("potential.bin", "w");
	if (!output_fp) {
		fprintf(stderr, "Failed to open output file '%s'.\n",
			"potential.bin");
		return;
	}

	solver_save_results(solver, output_fp);
	fclose(output_fp);

	output_fp = fopen("flux.bin", "w");
	if (output_fp) {
		solver_save_velocity(solver, output_fp);
	} else {
		fprintf(stderr,
			"Failed to open the velocity file for writing.\n");
		return;
	}
	fclose(output_fp);

	output_fp = fopen("output.xdmf", "w");
	if (output_fp) {
		write_xdmf(output_fp, solver->problem->width,
			   solver->problem->height);
		fclose(output_fp);
	} else {
		fprintf(stderr, "Failed to write the XDMF file.\n");
	}
}

int main(int argc, char const *argv[])
{
	printf("Diffusion Solver 0.0.1\n");
	print_double_usage();

	if (argc != 2) {
		print_usage();
		return 1;
	}

	const char *input_file_path = argv[1];
	const char *output_file_path = argv[2];

	/* Find and open the input file. */
	FILE *input_fp = fopen(input_file_path, "r");
	if (!input_fp) {
		fprintf(stderr, "Failed to open the input file.\n");
		return 1;
	}

	struct Problem *problem = problem_from_file(input_fp);
	if (!problem) {
		fprintf(stderr, "Failed to parse the input file.\n");
		fclose(input_fp);
		return 1;
	}

	fclose(input_fp);

	/* Shouldn't fail unless the memory allocation fails and thus we don't
	 * do any error checking. */
	struct Solver *solver = solver_from_problem(problem);
	assert(solver != NULL);

	struct PerformanceCounter *pf = performance_counter_new();
	performance_counter_start(pf);

	solver_solve(solver);

	performance_counter_stop(pf);

	performance_counter_set_iteration_count(
		pf, solver_get_iteration_count(solver));

	performance_counter_print_statistics(pf);
	performance_counter_free(pf);

	save_results(solver);

	solver_free(solver);

	return 0;
}
