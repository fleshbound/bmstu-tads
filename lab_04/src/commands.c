#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "arr_stack.h"
#include "commands.h"
#include "errors.h"
#include "input.h"
#include "stack.h"

#ifndef NMAX
#define NMAX 3000
#endif

#define ITERATIONS 20
#define F_COUNT 5
#define PRINT_MSG(n) fprintf(stdout, MSG_COM##n"\n")

static stack_t stack_1, stack_2, test_stack;
static arr_stack_t test_arr_stack;

void ignore(void *n)
{
    if (n) return;
}

void command_add_arr(void)
{

}

void null_data(int *data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
        data[i] = 0;
}

void command_show_rules(void)
{
    fprintf(stdout, MSG_RULES);
}

void command_show_menu(void)
{
    PRINT_MSG(1);
    PRINT_MSG(2);
    PRINT_MSG(3);
    PRINT_MSG(4);
    PRINT_MSG(5);
    PRINT_MSG(6);
    PRINT_MSG(7);
    PRINT_MSG(8);
    PRINT_MSG(9);
    PRINT_MSG(10);
    PRINT_MSG(11);
    PRINT_MSG(12);
    PRINT_MSG(13);
    PRINT_MSG(14);
    PRINT_MSG(15);
    PRINT_MSG(0);
}

int _fill_stack(stack_t *stack)
{
    char file_name[LEN_FILENAME];
    
    if (input_filename(file_name))
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL)
    {
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    *stack = fget_stack(f_curr);
    
    if (fclose(f_curr) == EOF)
    {
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    if (*stack == NULL)
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS; 
}

void command_fill_stacks_from_file(void)
{
    if ((stack_1 != NULL) || (stack_2 != NULL))
        command_clear_all();

    puts("Заполнение 1-го стека...");    
    int rc = _fill_stack(&stack_1);

    if (rc)
        return;
    else
        puts("Стек был успешно заполнен");

    /* command_show_stacks(); */
    puts("");

    puts("Заполнение 2-го стека...");
    rc = _fill_stack(&stack_2);

    if (rc)
        return;
    else
        puts("Стек был успешно заполнен");
}

void command_show_stacks(void)
{
    puts("\tСТЕК 1:");

    if (stack_1 == NULL)    
        show_err(ERR_EMPTY);
    else
        show_stack(stack_1);

    puts("");

    puts("\tСТЕК 2:");

    if (stack_2 == NULL)    
        show_err(ERR_EMPTY);
    else
        show_stack(stack_2);
}

int _add_new_elem(stack_t stack)
{
    if (get_size(stack) == get_max_size(stack))
    {
        show_err(ERR_FULL);
        return ERR_FULL;
    }

    int new;

    printf("Введите целое число: ");

    if (fscanf(stdin, "%d", &new) != READ_COUNT)
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    fgetc(stdin);

    push(stack, new);

    return EXIT_SUCCESS;
}

void command_add_test(void)
{
    if (test_stack == NULL)
    {
        puts("Стек пуст, создается новый...");
        test_stack = create_stack(MAX_STACK_SIZE);

        if (test_stack == NULL)
        {
            show_err(ERR_MEM);
            return;
        }
    }

    puts("Вставка в тестовый стек на основе списка...");
    
    if (_add_new_elem(test_stack))
            return;

    printf("Элемент был успешно вставлен;\nновый адрес: %p\n", get_top_address(test_stack));
}

void command_del_test(void)
{
    if (test_stack == NULL)       
    {
        show_err(ERR_EMPTY);
        return;
    }

    puts("Удаление из спискового стека...");

    void *ptr = get_top_address(test_stack);
    printf("Элемент %d был успешно удален;\nосвобожденный адрес: %p\n", pop(test_stack), ptr);
}

void command_arr_add_test(void)
{
    int new_size;

    if ((test_arr_stack == NULL) || (get_arr_size(test_arr_stack) == 0))
    {
        if (test_arr_stack != NULL)
            delete_arr_stack(&test_arr_stack);

        puts("Стек пуст, создание нового...");
        printf("Введите макс. размер: ");
        if ((fscanf(stdin, "%d", &new_size) != 1) && (new_size <= 0) && (new_size >= 1000))
        {
            puts("Ошибка: Неверные данные");
            return;
        }
        fgetc(stdin);

        test_arr_stack = create_arr_stack(new_size); 
    }

    if ((int)get_arr_size(test_arr_stack) <= get_arr_top(test_arr_stack))
    {
        puts("Ошибка: Переполнение");
        return;
    }

    int new_elem;

    puts("Добавление элемента...");
    printf("Введите значение элемента: ");

    if (fscanf(stdin, "%d", &new_elem) != 1)
    {
        puts("Ошибка: Неверный ввод");
        return;
    }
    fgetc(stdin);

    arr_push(test_arr_stack, new_elem);
    puts("Элемент был успешно добавлен");
}

void command_arr_del_test(void)
{
    if ((test_arr_stack == NULL) || (get_arr_top(test_arr_stack) <= 0))
    {
        puts("Ошибка: Стек пуст");
        return;
    }

    printf("Элемент %d был успешно удален\n", arr_pop(test_arr_stack));
}

void command_show_test(void)
{
    puts("СПИСОК:");
    if (test_stack == NULL)    
        show_err(ERR_EMPTY);
    else
        show_stack(test_stack);

    puts("");

    puts("\tМАССИВ:");

    if ((test_arr_stack == NULL) || (get_arr_top(test_arr_stack) == 0))
        show_err(ERR_EMPTY);
    else
        show_arr_stack(test_arr_stack);
}

void command_clear_test(void)
{
    puts("Очистка спискового...");
    if (test_stack == NULL)
    {
        show_err(ERR_EMPTY);
    }
    else
        delete_stack(&test_stack);

    puts("Очистка на основе массива...");
    if (test_arr_stack == NULL)
    {
        show_err(ERR_EMPTY);
    }
    else
        delete_arr_stack(&test_arr_stack);
}

void command_add(void)
{
    puts("1) в 1-й стек");
    puts("2) в 2-й стек");

    printf("Введите номер варианта: ");

    int var = 0;

    if ((fscanf(stdin, "%d", &var) != READ_COUNT)
        || ((var != VAR_1) && (var != VAR_2)))
    {
        show_err(ERR_DATA);
        return;
    }

    fgetc(stdin);

    if (var == 1)
    {
        if (stack_1 == NULL)
        {
            puts("Стек пуст, создается новый...");
            stack_1 = create_stack(MAX_STACK_SIZE);

            if (stack_1 == NULL)
            {
                show_err(ERR_MEM);
                return;
            }
        }

        puts("Вставка в 1-й стек...");

        if (_add_new_elem(stack_1))
            return;
    }
    else if (var == 2)
    {
        if (stack_2 == NULL)
        {
            puts("Стек пуст, создается новый...");
            stack_2 = create_stack(MAX_STACK_SIZE);

            if (stack_2 == NULL)
            {
                show_err(ERR_MEM);
                return;
            }
        }

        puts("Вставка во 2-й стек...");

        if (_add_new_elem(stack_2))
            return;
    }
    
    puts("Элемент был успешно добавлен");
}

void command_del(void)
{
    puts("1) из 1-го стека");
    puts("2) из 2-го стека");

    printf("Введите номер варианта: ");

    int var = 0;

    if ((scanf("%d", &var) != READ_COUNT)
        || ((var != VAR_1) && (var != VAR_2)))
    {
        show_err(ERR_DATA);
        return;
    }

    fgetc(stdin);
    
    if (var == 1)
    {
        if ((stack_1 == NULL) || (get_size(stack_1) == 0))
        {
            show_err(ERR_EMPTY);
            return;
        }

        puts("Удаление из 1-го стека...");
        printf("Элемент %d был успешно удален\n", pop(stack_1));
    }
    else if (var == 2)
    {
        if ((stack_2 == NULL) || (get_size(stack_2) == 0))
        {
            show_err(ERR_EMPTY);
            return;
        }
        
        puts("Удаление из 2-го стека...");
        printf("Элемент %d был успешно удален\n", pop(stack_2));
    }
}

stack_t get_sorted_stack(stack_t stack, long long unsigned *const sort_time)
{
    stack_t sorted_stack = create_stack(get_max_size(stack));

    if (sorted_stack == NULL)
        return NULL;

    int tmp;

    long long unsigned curr_time = clock();

    while (get_size(stack) != 0)
    {
        tmp = pop(stack);

        while ((get_size(sorted_stack) != 0) && (get_top_value(sorted_stack) > tmp))
            push(stack, pop(sorted_stack));

        push(sorted_stack, tmp);
    }

    curr_time = clock() - curr_time;

    if (sort_time != NULL)
        *sort_time = curr_time;

    return sorted_stack;
}

arr_stack_t get_sorted_arr_stack(arr_stack_t stack, long long unsigned *const sort_time)
{
    arr_stack_t sorted_stack = create_arr_stack(get_arr_size(stack));

    if (sorted_stack == NULL)
        return NULL;

    int tmp;

    long long unsigned curr_time = clock();

    while (get_arr_top(stack) != 0)
    {
        tmp = arr_pop(stack);

        while ((get_arr_top(sorted_stack) != 0) && (get_arr_top_value(sorted_stack) > tmp))
            arr_push(stack, arr_pop(sorted_stack));

        arr_push(sorted_stack, tmp);
    }

    curr_time = clock() - curr_time;

    if (sort_time != NULL)
        *sort_time = curr_time;

    return sorted_stack;
}

stack_t get_merged_stack(stack_t s1, stack_t s2, long long unsigned *const merge_time)
{
    stack_t merged_stack = create_stack(get_max_size(s1));

    if (merged_stack == NULL)
        return NULL;

    long long unsigned curr_time = clock();

    while (get_size(s1) != 0)
        push(merged_stack, pop(s1));

    while (get_size(s2) != 0)
        push(merged_stack, pop(s2));

    curr_time = clock() - curr_time;

    if (merge_time != NULL)
        *merge_time = curr_time;

    return merged_stack;
}

void command_sort_data(void)
{
    if ((stack_1 == NULL) || (stack_2 == NULL)
        || (get_size(stack_1) == 0) || (get_size(stack_2) == 0))
    {
        show_err(ERR_EMPTY);
        return;
    }

    puts("Выполняется сортировка стеков...");
    stack_t merged = get_merged_stack(stack_1, stack_2, NULL);

    if (merged == NULL)
    {
        show_err(ERR_MEM);
        return;
    }

    delete_stack(&stack_1);
    stack_1 = merged;
    puts("Слияние прошло успешно");

    puts("Выполняется сортировка стека...");
    stack_t sorted = get_sorted_stack(merged, NULL);
    delete_stack(&stack_1);
    delete_stack(&stack_2);

    if (sorted == NULL)
    {
        show_err(ERR_MEM);
        return;
    }

    stack_1 = sorted;
    puts("Сортировка прошла успешно, 1-й стек перезаписан");

    puts("\tРЕЗУЛЬТАТ СОРТИРОВКИ");
    show_stack(stack_1);
}

void command_sort_diff(void)
{
    char files[5][LEN_FILENAME] = { 
        "./data/500.txt", 
        "./data/1000.txt",
        "./data/2000.txt", 
        "./data/500_1.txt", 
        "./data/500__1.txt"
    };

    FILE *f = NULL;
    stack_t tmp_stack = NULL, tmp_sorted_stack = NULL;
    arr_stack_t tmp_arr_stack = NULL, tmp_sorted_arr_stack = NULL;
    long long unsigned curr_time = 0, sum_time, arr_sum_time;
    size_t curr_size, arr_curr_size;

    puts("               Время (мкс)    Размер (Б)   ");

    for (size_t file_ind = 0; file_ind < 5; file_ind++)
    {
        f = fopen(files[file_ind], "rt");
        sum_time = 0;
        arr_sum_time = 0;

        printf("\t%s\n", files[file_ind]);

        for (size_t i = 0; i < ITERATIONS; i++)
        {
            tmp_stack = fget_stack(f);
            tmp_arr_stack = fget_arr_stack(f);

            curr_size = get_struct_size(tmp_stack);
            arr_curr_size = get_arr_struct_size(tmp_arr_stack);

            tmp_sorted_stack = get_sorted_stack(tmp_stack, &curr_time);
            sum_time += curr_time;

            tmp_sorted_arr_stack = get_sorted_arr_stack(tmp_arr_stack, &curr_time);
            arr_sum_time += curr_time;

            delete_stack(&tmp_stack);
            delete_stack(&tmp_sorted_stack);
            delete_arr_stack(&tmp_arr_stack);
            delete_arr_stack(&tmp_sorted_arr_stack);
        }

        sum_time = sum_time / (ITERATIONS * CLOCKS_PER_SEC / 1000000LLU);
        arr_sum_time = arr_sum_time / (ITERATIONS * CLOCKS_PER_SEC / 1000000LLU);

        printf("linked list: %15llu %15zu\n", sum_time, curr_size);
        printf("vector     : %15llu %15zu\n", arr_sum_time, arr_curr_size);

        fclose(f);
    }
}

void command_clear_all(void)
{
    puts("Очистка 1-го стека...");

    if (stack_1 == NULL)
    {
        show_err(ERR_EMPTY);
    }
    else
        delete_stack(&stack_1);

    puts("Очистка 2-го стека...");

    if (stack_2 == NULL)
    {
        show_err(ERR_EMPTY);
    }
    else
        delete_stack(&stack_2);
}

