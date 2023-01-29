#include "efficiency.h"

void print_insertion_efficiency(int elements_amount)
{
    clock_t array_queue_start = 0;
    clock_t array_queue_end = 0;
    clock_t list_queue_start = 0;
    clock_t list_queue_end = 0;
    array_queue_t *array_queue;
    list_queue_t *list_queue;
    int value;

    array_queue = new_array_queue(elements_amount);
    list_queue = new_list_queue();

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        for (size_t j = 0; j < (size_t) elements_amount; j++)
        {
            value = rand();
            array_queue_start += clock();
            array_queue_push(array_queue, value);
            array_queue_end += clock();
        }

        for (size_t j = 0; j < (size_t) elements_amount; j++)
        {
            value = rand();
            list_queue_start += clock();
            list_queue_push(list_queue, value);
            list_queue_end += clock();
        }
    }

    puts("ДОБАВЛЕНИЕ");
    printf("ЭЛЕМЕНТОВ: %d; ИТЕРАЦИЙ: %d\n", elements_amount, ITERATIONS);
    printf("Массив: %.3lf (тиков процессора)\n", (double)(array_queue_end - array_queue_start) / ITERATIONS);
    printf("Список: %.3lf (тиков процессора)\n", (double)(list_queue_end - list_queue_start) / ITERATIONS);

    free_array_queue(array_queue);
    free_list_queue(list_queue);
}

void print_deletion_efficiency(int elements_amount)
{
    clock_t array_queue_start = 0;
    clock_t array_queue_end = 0;
    clock_t list_queue_start = 0;
    clock_t list_queue_end = 0;
    array_queue_t *array_queue;
    list_queue_t *list_queue;
    int value;

    array_queue = new_array_queue(elements_amount);
    list_queue = new_list_queue();

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        for (size_t j = 0; j < (size_t) elements_amount; j++)
        {
            value = rand();
            array_queue_push(array_queue, value);
            list_queue_push(list_queue, value);
        }

        array_queue_start += clock();
        
        for (size_t j = 0; j < (size_t) elements_amount; j++)
            array_queue_pop(array_queue, &value);
        
        array_queue_end += clock();

        list_queue_start += clock();
        
        for (size_t j = 0; j < (size_t) elements_amount; j++)
            list_queue_pop(list_queue, &value);
        
        list_queue_end += clock();
    }

    puts("УДАЛЕНИЕ");
    printf("ЭЛЕМЕНТОВ: %d; ИТЕРАЦИЙ: %d\n", elements_amount, ITERATIONS);
    printf("Массив: %.3lf (тиков процессора)\n", (double)(array_queue_end - array_queue_start) / ITERATIONS);
    printf("Список: %.3lf (тиков процессора)\n", (double)(list_queue_end - list_queue_start) / ITERATIONS);

    free_array_queue(array_queue);
    free_list_queue(list_queue);
}

void print_time_efficiency(int elements_amount)
{
    print_insertion_efficiency(elements_amount);
    print_newline();
    print_deletion_efficiency(elements_amount);
    print_newline();
}

void print_memory_efficiency(int elements_amount)
{
    puts("РАЗМЕР");
    printf("Массив: %lu (Б)\n", get_array_queue_size_bytes(elements_amount));
    printf("Список: %lu (Б)\n", get_list_queue_size_bytes(elements_amount));
}

void compare_efficiency(void)
{
    int rc;
    int elements_amount;

    printf("Кол-во эл-тов [%d;%d]:", 0, MAX_DEMO_VALUE);
    rc = scan_integer(&elements_amount, 0, MAX_DEMO_VALUE);

    if (rc) return;

    print_newline();
    print_time_efficiency(elements_amount);
    print_memory_efficiency(elements_amount);
}
