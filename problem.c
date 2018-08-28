#include "problem.h"
#include <stdlib.h>
#include <string.h>

static void read_grid_definition(FILE *fp, struct Problem *problem)
{
	char buffer[256];
	fgets(buffer, 256, fp);

	char field_name_buffer[256];

	sscanf(buffer, "%d %d %f %f %s\n", &problem->width, &problem->height,
	       &problem->physical_width, &problem->physical_height,
	       field_name_buffer);

	size_t name_length = strlen(field_name_buffer);
	problem->field_name = (char *)calloc(name_length, sizeof(char));
	strcpy(problem->field_name, field_name_buffer);
}

struct Problem *problem_from_file(FILE *fp)
{
	struct Problem *problem = (struct Problem *)malloc(sizeof(*problem));

	read_grid_definition(fp, problem);

	return problem;
}

void problem_free(struct Problem *problem)
{
	free(problem->field_name);
	free(problem);
}
