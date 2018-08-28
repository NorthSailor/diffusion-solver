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

static void read_boundary_conditions(FILE *fp, struct Problem *problem)
{
	char buffer[256];
	struct BoundaryCondition **next = &problem->boundary_conditions;

	while (fgets(buffer, 256, fp)) {
		if (buffer[0] == '\n') {
			/* Empty line - skip. */
			continue;
		}

		(*next) = (struct BoundaryCondition *)malloc(sizeof(**next));
		struct BoundaryCondition *b = *next;

		char edge_buffer[5];
		sscanf(buffer, "%s %d %d %f", edge_buffer, &b->start, &b->end,
		       &b->value);

		if (strncmp(edge_buffer, "X+", 2) == 0)
			b->boundary = BC_XPLUS;
		else if (strncmp(edge_buffer, "X-", 2) == 0)
			b->boundary = BC_XMINUS;
		else if (strncmp(edge_buffer, "Y+", 2) == 0)
			b->boundary = BC_YPLUS;
		else if (strncmp(edge_buffer, "Y-", 2) == 0)
			b->boundary = BC_YMINUS;
		else {
			fprintf(stderr, "Invalid boundary designation: '%s'\n",
				edge_buffer);
			exit(1);
		}

		next = &(*next)->next;
	}
}

struct Problem *problem_from_file(FILE *fp)
{
	struct Problem *problem = (struct Problem *)malloc(sizeof(*problem));
	problem->boundary_conditions = NULL;

	read_grid_definition(fp, problem);
	read_boundary_conditions(fp, problem);

	return problem;
}

void problem_free(struct Problem *problem)
{
	struct BoundaryCondition *b = problem->boundary_conditions;
	while (b != NULL) {
		free(b);
		b = b->next;
	}

	free(problem->field_name);
	free(problem);
}
