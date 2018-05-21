#include <stdlib.h>
#include <string.h>
#include "matfile.h"

// The size for the matfile header should add up to 128
#define MATFILE_HEADER_TEXT_SIZE 116
#define MATFILE_HEADER_SUBSYS_SIZE 8
#define MATFILE_HEADER_VERSION_SIZE 2
#define MATFILE_HEADER_ENDIAN_SIZE 2

#define MATFILE_DATA_TYPE_INT32_SIZE 4
#define MATFILE_DATA_TYPE_UINT32_SIZE 4

// MAT-file format is big endian

FILE *matfile_create(const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    
    if(fp == NULL)
        return NULL;

    char header_text[MATFILE_HEADER_TEXT_SIZE] = { 0 };
    // TODO: Include more info like when created and on what platform
    snprintf(header_text, MATFILE_HEADER_TEXT_SIZE, "MATLAB 5.0 MAT-file\n");
    fwrite(header_text, sizeof(char), MATFILE_HEADER_TEXT_SIZE, fp);
    
    char subsys[MATFILE_HEADER_SUBSYS_SIZE] = { 0 };
    fwrite(subsys, sizeof(char), MATFILE_HEADER_SUBSYS_SIZE, fp);

    char version[MATFILE_HEADER_VERSION_SIZE] = {0, 1};
    fwrite(version, sizeof(char), MATFILE_HEADER_VERSION_SIZE, fp);

    char endian[MATFILE_HEADER_ENDIAN_SIZE] = {'I', 'M'};
    fwrite(endian, sizeof(char), MATFILE_HEADER_ENDIAN_SIZE, fp);
    
    return fp;
}

void matfile_add_array(const char *array_name, void *data, int data_len, int data_element_type, FILE *fp)
{
    switch(data_element_type)
    {
        case MATFILE_DATA_TYPE_INT32:;
            // Tag
            int data_type = MATFILE_DATA_TYPE_MATRIX;
            fwrite(&data_type, sizeof(int), 1, fp);

            // TODO: Need to calculate size before hand to include padded bytes
            int matrix_size = (data_len * MATFILE_DATA_TYPE_INT32_SIZE) + 56 + 15;
            fwrite(&matrix_size, sizeof(int), 1, fp);

            // Array Flags
            int array_type = MATFILE_DATA_TYPE_UINT32;
            fwrite(&array_type, sizeof(int), 1, fp);
            
            int array_size = 2 * MATFILE_DATA_TYPE_UINT32_SIZE;
            fwrite(&array_size, sizeof(int), 1, fp);

            char array_flags[8] = { 0 };
            array_flags[2] = 1 << 5;
            array_flags[3] = MATFILE_CLASS_INT32;
            fwrite(&array_flags, sizeof(char), 8, fp);

            // Dimensions Array
            int dimension_type = MATFILE_DATA_TYPE_INT32;
            fwrite(&dimension_type, sizeof(int), 1, fp);

            int dimension_size = 8;
            fwrite(&dimension_size, sizeof(int), 1, fp);

            int dimensions[] = {1, data_len};
            fwrite(&dimensions, sizeof(int), 2, fp);

            // Array Name
            int array_name_type = MATFILE_DATA_TYPE_INT8;
            fwrite(&array_name_type, sizeof(int), 1, fp);

            int array_name_size = matfile_get_aligned_size(strlen(array_name), 8);
                
            fwrite(&array_name_size, sizeof(int), 1, fp);

            char *aligned_array_name = calloc(array_name_size, sizeof(char));
            snprintf(aligned_array_name, array_name_size, "%s", array_name);
            fwrite(aligned_array_name, sizeof(char), array_name_size, fp);
            free(aligned_array_name);

            // Real part
            int real_part_type = MATFILE_DATA_TYPE_INT32;
            fwrite(&real_part_type, sizeof(int), 1, fp);

            int real_part_size = data_len * MATFILE_DATA_TYPE_INT32_SIZE;
            fwrite(&real_part_size, sizeof(int), 1, fp);

            int real_part_aligned_size = matfile_get_aligned_size(data_len, 2);
            int *aligned_data = calloc(real_part_aligned_size, sizeof(int));
            memcpy(aligned_data, (int *)data, data_len * sizeof(int));
            fwrite(aligned_data, sizeof(int), real_part_aligned_size, fp);
            free(aligned_data);
            
            // Imginary part (optional)
            break;
    }
}

int matfile_get_aligned_size(int size, int width)
{
    int aligned_size = size;
    int remainder = size % width;

    if(remainder != 0)
        aligned_size += width - remainder;

    return aligned_size;
}
