#include "problem.h"
#include <stdlib.h>
#include <string.h>

static void read_grid_definition(FILE *fp, struct Problem *problem)
{
	char buffer[256];
	if (!fgets(buffer, 256, fp)) {
		fprintf(stderr, "Failed to read the grid definition line.\n");
		exit(1);
	}

	char field_name_buffer[256];

	sscanf(buffer, "%d %d %" NUMBER_FMT " %" NUMBER_FMT " %s\n",
	       &problem->width, &problem->height, &problem->physical_width,
	       &problem->physical_height, field_name_buffer);

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

		char edge_buffer[4];
		char type_buffer[16];
		sscanf(buffer, "%s %s %d %d %" NUMBER_FMT, edge_buffer,
		       type_buffer, &b->start, &b->end, &b->value);

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

		if (strcmp(type_buffer, "DIRICHLET") == 0)
			b->type = BC_DIRICHLET;
		else if (strcmp(type_buffer, "NEUMANN") == 0)
			b->type = BC_NEUMANN;
		else {
			fprintf(stderr,
				"Unrecognized boundary condition type: '%s'\n",
				type_buffer);
			exit(1);
		}

		b->next = NULL; /* Make sure we won't segfault if this is the
				   last one .*/

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
		struct BoundaryCondition *prev = b;
		b = b->next;
		free(prev);
	}

	free(problem->field_name);
	free(problem);
}
