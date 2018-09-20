#ifndef PROBLEM_H
#define PROBLEM_H

#include <inttypes.h>
#include <stdio.h>
#include "number.h"

#define BC_XPLUS 0
#define BC_XMINUS 1
#define BC_YPLUS 2
#define BC_YMINUS 3

#define BC_DIRICHLET 0
#define BC_NEUMANN 1

struct BoundaryCondition {
	struct BoundaryCondition *next;

	int boundary;
	int type;
	int start;
	int end;

	number_t value;
};

struct Problem {
	uint32_t width;
	uint32_t height;

	number_t physical_width;
	number_t physical_height;

	char *field_name;

	struct BoundaryCondition *boundary_conditions;
};

struct Problem *problem_from_file(FILE *fp);
void problem_free(struct Problem *problem);

#endif /* PROBLEM_H */
