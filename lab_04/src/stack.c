#include <stdlib.h>

#include "stack.h"

typedef struct stack_elem *stack_elem_t;

struct stack_elem
{
    stack_elem_t next;
    int value;
};

struct stack
{
    stack_elem_t top;
    size_t size;
    size_t max_size;
};

size_t get_struct_size(stack_t stack)
{
    return stack->size * sizeof(struct stack_elem) + sizeof(size_t) * 2;
}

int get_top_value(stack_t stack)
{
    return stack->top->value;
}

void free_elem(stack_elem_t elem)
{
    free(elem);
}

void *get_top_address(stack_t stack)
{
    return (void *)stack->top;
}

stack_elem_t create_elem(const int value)
{
    stack_elem_t elem = malloc(sizeof(struct stack_elem));

    if (elem != NULL)
    {
        elem->value = value;
        elem->next = NULL;
    }

    return elem;
}

int pop(stack_t stack)
{
    int value = stack->top->value;
    stack_elem_t tmp = stack->top->next;

    free_elem(stack->top);
    stack->top = tmp;
    stack->size--;

    return value;
}

void push(stack_t stack, const int value)
{
    stack_elem_t elem = create_elem(value);

    if (elem == NULL)
        return;

    elem->next = stack->top;
    stack->top = elem;
    stack->size++;
}

stack_t create_stack(const size_t max_size)
{
    stack_t stack = malloc(sizeof(struct stack));

    if (stack != NULL)
    {
        stack->top = NULL;
        stack->size = 0;
        stack->max_size = max_size;
    }

    return stack;
}

size_t get_size(stack_t stack)
{
    return stack->size;
}

size_t get_max_size(stack_t stack)
{
    return stack->max_size;
}

void delete_stack(stack_t *stack)
{
    while ((*stack)->top != NULL)
        pop(*stack);

    free(*stack);
    *stack = NULL;
}

void show_stack(stack_t stack)
{
    if (stack->size == 0)
    {
        puts("Стек пуст");
        return;
    }
    
    printf("Максимальный размер: %zu\n", stack->max_size);
    printf("Количество элементов: %zu\n", stack->size);
    
    puts("Содержимое:");
    printf("<- [ ");

    stack_elem_t curr = stack->top;

    while (curr != NULL)
    {
        printf("%d ", curr->value);
        curr = curr->next;
    }

    puts("]");
}
