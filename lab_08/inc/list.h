#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>

#include "errors.h"
#include "input.h"
#include "matrix.h"

typedef struct list_elem *list_data_t;

typedef struct list
{
    size_t size;
    list_data_t *nodes;
} list_t;

list_data_t create_list_elem(const size_t vertex, const size_t dist);
void free_list(list_t *list);
list_data_t *alloc_list_data(const size_t num_of_nodes);
void print_list(list_t list);
int list_get_min_sum_city(list_t list, result_t *result);
int fget_graph_list(FILE *const f, list_t *graph_list);
void print_dot_list(FILE *const fviz, list_t list);
size_t get_list_struct_size(list_t list);

#endif
