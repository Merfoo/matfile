#include <stdio.h>
#include "matfile.h"

int main(int argc, const char *argv[])
{
    printf("Hello world!\n");

    FILE *fp = matfile_create("test.mat");
    int data[] = {9, 2, 3, 4, 5, 6, 7, 8, 9};
    matfile_add_array("123456789", data, sizeof(data) / sizeof(int), MATFILE_DATA_TYPE_INT32, fp);
    fclose(fp);

    return 0;
}
