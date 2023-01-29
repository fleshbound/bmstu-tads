#ifndef _ARR_STACK_H_
#define _ARR_STACK_H_

#include <stdio.h>

typedef struct arr_stack *arr_stack_t;

int arr_pop(arr_stack_t stack);
void arr_push(arr_stack_t stack, const int value);
arr_stack_t create_arr_stack(const size_t size);
void delete_arr_stack(arr_stack_t *stack);
size_t get_arr_size(arr_stack_t stack);
int get_arr_top_value(arr_stack_t stack);
size_t get_arr_struct_size(arr_stack_t stack);
int get_arr_top(arr_stack_t stack);
void show_arr_stack(arr_stack_t stack);

#endif
