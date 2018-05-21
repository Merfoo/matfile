#ifndef MATFILE_H
#define MATFILE_H

#include <stdio.h>

// Data Types
#define MATFILE_DATA_TYPE_INT8 1
#define MATFILE_DATA_TYPE_INT32 5
#define MATFILE_DATA_TYPE_UINT32 6
#define MATFILE_DATA_TYPE_MATRIX 14

// Array Type (Class)
#define MATFILE_CLASS_INT32 12

FILE *matfile_create(const char *filename);

void matfile_add_array(const char *array_name, void *data, int data_len, int data_element_type, FILE *fp);

int matfile_get_aligned_size(int size, int width);

#endif /* MATFILE_H */
