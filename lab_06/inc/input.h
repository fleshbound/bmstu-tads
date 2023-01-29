#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#define LEN_FILENAME 255
#define LEN_COM 255

#define VERBOSE 1
#define QUIET 0

#define READ_COUNT 1

int input_command(void);
int input_filename(char *const name);
void clear_str(char *const str);
int input_node(int *new_key);
int finput_node(FILE *const f, int *const new_key, const int code);

#endif
