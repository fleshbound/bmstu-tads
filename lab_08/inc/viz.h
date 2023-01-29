#ifndef TREEVIZ_H
#define TREEVIZ_H

#include "list.h"
#include "matrix.h"

int open_dot(const char *filename);
int close_dot(void);
void matrix_to_dot(char *const graph_name, my_matrix_t matrix);
void list_to_dot(char *const graph_name, list_t list);

#endif
