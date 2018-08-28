#include <stdio.h>
#include <assert.h>
#include "matrix.h"
#include "problem.h"
#include "solver.h"

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
	printf("Usage:\n\tdiffusion INPUT-FILE OUTPUT-FILE\n");
}

int main(int argc, char const *argv[])
{
	printf("Diffusion Solver 0.0.1\n");
	print_double_usage();

	if (argc != 3) {
		print_usage();
		return 1;
	}

	const char *input_file_path = argv[1];
	const char *output_file_path = argv[2];

	/* Find and open the input file. */
	FILE *input_fp = fopen(input_file_path, "r");
	if (!input_fp) {
		printf("Failed to open the input file.\n");
		return 1;
	}

	struct Problem *problem = problem_from_file(input_fp);
	if (!problem) {
		printf("Failed to parse the input file.\n");
		fclose(input_fp);
		return 1;
	}

	fclose(input_fp);

	/* Shouldn't fail unless the memory allocation fails and thus we don't
	 * do any error checking. */
	struct Solver *solver = solver_from_problem(problem);
	assert(solver != NULL);

	solver_solve(solver);

	solver_free(solver);

	return 0;
}
