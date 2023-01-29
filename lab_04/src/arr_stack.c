#include <stdlib.h>

#include "arr_stack.h"

struct arr_stack
{
    int *data;
    size_t size;
    int top;
};

size_t get_arr_struct_size(arr_stack_t stack)
{
    return stack->size * sizeof(int) + sizeof(size_t) + sizeof(int);
}

size_t get_arr_size(arr_stack_t stack)
{
    return stack->size;
}

int get_arr_top(arr_stack_t stack)
{
    return stack->top;
}

int get_arr_top_value(arr_stack_t stack)
{
    return stack->data[stack->top - 1];
}

int arr_pop(arr_stack_t stack)
{
    return stack->data[stack->top-- - 1];
}

void arr_push(arr_stack_t stack, const int value)
{
    stack->data[stack->top++] = value;
}

arr_stack_t create_arr_stack(const size_t size)
{
    arr_stack_t stack = malloc(sizeof(struct arr_stack));

    if (stack != NULL)
    {
        stack->size = size;
        stack->top = 0;

        stack->data = calloc(size, sizeof(int));

        if (stack->data == NULL)
            free(stack);
    }

    return stack;
}

void delete_arr_stack(arr_stack_t *stack)
{
    free((*stack)->data);
    free(*stack);
    *stack = NULL;
}

void show_arr_stack(arr_stack_t stack)
{
    printf("Максимальный размер: %zu\n", stack->size);
    puts("Вершина слева");
    for (int i = stack->top - 1; i >= 0; i--)
    {
        printf("%d ", stack->data[i]);
    }
}
