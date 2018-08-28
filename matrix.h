#ifndef MATRIX_H
#define MATRIX_H

#include "number.h"
#include <stdio.h>

struct Matrix {
	number_t *data;
	int rows, columns;
};

struct Matrix *matrix_new(int rows, int columns);

void matrix_free(struct Matrix *matrix);
number_t matrix_get(struct Matrix *matrix, int row, int column);
void matrix_set(struct Matrix *matrix, int row, int column, number_t value);
void matrix_write_to_file(struct Matrix *matrix, FILE *fp);
void matrix_write_vector_field(struct Matrix *x_matrix, struct Matrix *y_matrix,
			       const char *path);

#endif /* MATRIX_H */
