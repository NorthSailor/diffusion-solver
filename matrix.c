#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

struct Matrix *matrix_new(int rows, int columns)
{
	struct Matrix *matrix = (struct Matrix *)malloc(sizeof(*matrix));
	matrix->rows = rows;
	matrix->columns = columns;
	matrix->data =
		(MATRIX_TYPE *)calloc(rows * columns, sizeof(MATRIX_TYPE));
	return matrix;
}

void matrix_free(struct Matrix *matrix)
{
	free(matrix->data);
	free(matrix);
}

MATRIX_TYPE matrix_get(struct Matrix *matrix, int row, int column)
{
	return matrix->data[row * matrix->columns + column];
}

void matrix_set(struct Matrix *matrix, int row, int column, MATRIX_TYPE value)
{
	matrix->data[row * matrix->columns + column] = value;
}

/**
 * Writes the contents of the matrix in a binary file.
 * @param matrix The matrix to save.
 * @param path The path to the output file.
 */
void matrix_write_to_file(struct Matrix *matrix, const char *path)
{
	FILE *file = fopen(path, "w");
	if (!file) {
		fprintf(stderr, "Failed to open output file: '%s'.\n", path);
		exit(1);
	}

	fwrite((void *)matrix->data, sizeof(MATRIX_TYPE),
	       matrix->rows * matrix->columns, file);

	fclose(file);
}

void matrix_write_vector_field(struct Matrix *x_matrix, struct Matrix *y_matrix,
			       const char *path)
{
	FILE *file = fopen(path, "w");
	if (!file) {
		fprintf(stderr, "Failed to open output file: '%s'.\n", path);
		exit(1);
	}

	/* We assume that the matrices have the same dimensions. */
	int size = x_matrix->rows * x_matrix->columns;
	for (int i = 0; i < size; i++) {
		fwrite(&x_matrix->data[i], sizeof(MATRIX_TYPE), 1, file);
		fwrite(&y_matrix->data[i], sizeof(MATRIX_TYPE), 1, file);
	}

	fclose(file);
}
