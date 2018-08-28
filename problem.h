#ifndef PROBLEM_H
#define PROBLEM_H

#include <inttypes.h>
#include "number.h"

struct Problem {
	uint32_t width;
	uint32_t height;

	number_t physical_width;
	number_t physical_height;

	char *field_name;
};

#endif /* PROBLEM_H */
