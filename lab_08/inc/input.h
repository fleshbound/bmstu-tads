#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>

#define LEN_FILENAME 255
#define LEN_COM 255
#define READ_COUNT 1

typedef struct result
{
    size_t vertex;
    size_t min_sum;
} result_t;

int fscanf_size(FILE *const f, size_t *const size);
int input_filename(char *const name);
int input_command(void);

#endif
