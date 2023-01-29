#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#include "arr_stack.h"
#include "stack.h"

#define LEN_FILENAME 255
#define LEN_COM 255

#define MAX_STACK_SIZE 5000

#define VERBOSE 1
#define QUIET 0

#define READ_COUNT 1

int input_command(void);
int input_filename(char *const name);
void clear_str(char *const str);
stack_t fget_stack(FILE *const f);
arr_stack_t fget_arr_stack(FILE *const f);
size_t get_int_count(FILE *const f);

#endif
