#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>

#include "input.h"

#define FALSE 0
#define TRUE 1

typedef int bool;
typedef struct my_matrix
{
    size_t **data;
    size_t rows;
    size_t cols;
} my_matrix_t;

void free_matrix(my_matrix_t *matrix);
my_matrix_t alloc_matrix(const size_t rows, const size_t cols);
void print_matrix(my_matrix_t matrix);
int matrix_get_min_sum_city(my_matrix_t matrix, result_t *result);
int fget_graph_matrix(FILE *const f, my_matrix_t *graph_matrix);
size_t *get_matrix_dijkstra(my_matrix_t graph_matrix, const size_t source_v);

#endif
