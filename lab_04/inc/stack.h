#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

typedef struct stack *stack_t;

int pop(stack_t stack);
void push(stack_t stack, const int value);
stack_t create_stack(const size_t max_size);
size_t get_size(stack_t stack);
size_t get_max_size(stack_t stack);
void delete_stack(stack_t *stack);
void show_stack(stack_t stack);
int get_top_value(stack_t stack);
size_t get_struct_size(stack_t stack);
void *get_top_address(stack_t stack);

#endif
