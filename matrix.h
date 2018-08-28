#ifndef MATRIX_H
#define MATRIX_H

#ifndef MATRIX_TYPE
#define MATRIX_TYPE double
#endif

struct Matrix {
	MATRIX_TYPE *data;
	int rows, columns;
};

struct Matrix *matrix_new(int rows, int columns);

void matrix_free(struct Matrix *matrix);
MATRIX_TYPE matrix_get(struct Matrix *matrix, int row, int column);
void matrix_set(struct Matrix *matrix, int row, int column, MATRIX_TYPE value);
void matrix_write_to_file(struct Matrix *matrix, const char *path);
void matrix_write_vector_field(struct Matrix *x_matrix, struct Matrix *y_matrix,
			       const char *path);

#endif /* MATRIX_H */
