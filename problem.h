#ifndef PROBLEM_H
#define PROBLEM_H

#include <inttypes.h>
#include <stdio.h>
#include "number.h"

struct Problem {
	uint32_t width;
	uint32_t height;

	number_t physical_width;
	number_t physical_height;

	char *field_name;
};

struct Problem *problem_from_file(FILE *fp);
void problem_free(struct Problem *problem);

#endif /* PROBLEM_H */
