#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "commands.h"
#include "errors.h"
#include "input.h"
#include "list.h"
#include "matrix.h"
#include "viz.h"

#ifndef NMAX
#define NMAX 3000
#endif

#define ITERATIONS 500
#define F_COUNT 5
#define PRINT_MSG(n) fprintf(stdout, MSG_COM##n"\n")

static my_matrix_t graph_matrix;
static list_t graph_list;
static char file_name_matrix[LEN_FILENAME];
static char file_name_list[LEN_FILENAME];

/* static char files[F_COUNT][LEN_FILENAME] = { "./data/500.txt", "./data/1000.txt", "./data/2000.txt", "./data/500_1.txt", "./data/500__1.txt" }; */
/* static size_t sizes[F_COUNT] = { 500, 1000, 2000, 500, 500 }; */

void ignore(void *n)
{
    if (n)
        return;
}

void command_show_rules(void)
{
    fprintf(stdout, MSG_RULES);
}

void command_show_menu(void)
{
    fprintf(stdout, "\nТЕКУЩИЙ ФАЙЛ ТАБЛИЦЫ: %s", (strlen(file_name_matrix) == 0) ? "Не выбран" : file_name_matrix );
    fprintf(stdout, "\nТЕКУЩИЙ ФАЙЛ СПИСКОВ: %s", (strlen(file_name_list) == 0) ? "Не выбран" : file_name_list );

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
    /* PRINT_MSG(11); */
    /* PRINT_MSG(12); */
    /* PRINT_MSG(13); */
    /* PRINT_MSG(14); */
    /* PRINT_MSG(15); */
    PRINT_MSG(0);
}

void reset_filenames(void)
{
    file_name_matrix[0] = '\0';
    file_name_list[0] = '\0';
}

int _set_filename_matrix(void)
{
    puts("Ввод файла таблицы...");

    if (input_filename(file_name_matrix))
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    FILE *f_curr = fopen(file_name_matrix, "rt");

    if (f_curr == NULL)
    {
        reset_filenames();
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    if (fclose(f_curr) == EOF)
    {
        reset_filenames();
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    printf("Файл таблицы был успешно задан\n");
    
    return EXIT_SUCCESS;
}

int _set_filename_list(void)
{
    puts("Ввод файла списков...");

    if (input_filename(file_name_list))
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    FILE *f_curr = fopen(file_name_list, "rt");

    if (f_curr == NULL)
    {
        reset_filenames();
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    if (fclose(f_curr) == EOF)
    {
        reset_filenames();
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    printf("Файл списков был успешно задан\n");
    
    return EXIT_SUCCESS;
}

void command_set_file(void)
{
    if (_set_filename_matrix()) return;
    
    if (_set_filename_list()) return;
}

void _show_graph(void)
{
    system("dot ./viz/graph.dot -Tpng > ./viz/graph.png");
    system("gwenview --title \"Current graph\" ./viz/graph.png");
}

int _fill_matrix(void)
{
    FILE *f = fopen(file_name_matrix, "rt");

    if (f == NULL)
    {
        /* reset_filenames(); */
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    int rc = fget_graph_matrix(f, &graph_matrix);    

    if (rc)
    {
        show_err(rc);
        /* reset_filenames(); */
        return rc;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        /* reset_filenames(); */
        return ERR_CLOSE;
    }

    return EXIT_SUCCESS;
}

int _fill_list(void)
{
    FILE *f = fopen(file_name_list, "rt");

    if (f == NULL)
    {
        /* reset_filenames(); */
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    int rc = fget_graph_list(f, &graph_list);    

    if (rc)
    {
        show_err(rc);
        /* reset_filenames(); */
        return rc;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        /* reset_filenames(); */
        return ERR_CLOSE;
    }

    return EXIT_SUCCESS;
}

void command_fill_structures(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }

    if ((graph_list.size > 0) || (graph_matrix.rows > 0))
    {
        puts("Ошибка: Структуры заполнены, требуется очистка");
        return;
    }

    puts("Начало загрузки таблицы смежности...");

    if (_fill_matrix())
        return;

    puts("Таблица смежности была успешно заполнена");
    puts("Начало загрузки списков ребер...");
    
    if (_fill_list())
        return;
    
    puts("Списки ребер был успешно заполнен");
}

void command_show_matrix(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }
    
    if (graph_matrix.rows == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    puts("Текущая таблица смежности:\n");
    print_matrix(graph_matrix);
}

void command_show_list(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }

    if (graph_list.size == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    puts("Текущие списки ребер:\n");
    print_list(graph_list);
}

void command_show_structures(void)
{
    command_show_matrix();
    command_show_list();
}

void command_show_graph_with_matrix(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }
    
    if (graph_matrix.rows == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    if (open_dot("./viz/graph.dot"))
        return;

    matrix_to_dot("matrix_graph", graph_matrix);

    if (close_dot())
        return;

    _show_graph();
}

void command_show_graph_with_list(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }
    
    if (graph_list.size == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    if (open_dot("./viz/graph.dot"))
        return;

    list_to_dot("list_graph", graph_list);

    if (close_dot())
        return;

    _show_graph();
}

void command_search_minsum_vertex(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }
    
    if (graph_list.size == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    result_t matrix_res = { .vertex = 0, .min_sum = 0 };
    result_t list_res = { .vertex = 0, .min_sum = 0 };

    puts("Начат поиск в таблице смежности...");
    int rc = matrix_get_min_sum_city(graph_matrix, &matrix_res);

    if (rc)
    {
        show_err(rc);
        return;
    }

    puts("Результаты по таблице");
    printf("Номер: %zu\nСумма: %zu\n", matrix_res.vertex + 1, matrix_res.min_sum);

    puts("Начат поиск в списках ребер...");
    rc = list_get_min_sum_city(graph_list, &list_res);

    if (rc)
    {
        show_err(rc);
        return;
    }

    puts("Результаты по спискам");
    printf("Номер: %zu\nСумма: %zu\n", list_res.vertex + 1, list_res.min_sum);
}

void command_clear_all(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }
    
    free_matrix(&graph_matrix);
    free_list(&graph_list);
    /* reset_filenames(); */

    puts("Данные были успешно очищены");
}

void command_show_efficiency(void)
{
    if ((strlen(file_name_matrix) == 0) || (strlen(file_name_list) == 0))
    {
        show_err(ERR_FILE);
        return;
    }

    if (graph_list.size == 0)
    {
        puts("Ошибка: Структуры не заполнены");
        return;
    }

    long long unsigned curr_time = 0, m_sum_time = 0, l_sum_time = 0;
    size_t m_size = 0, l_size = 0;
    result_t res;

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        curr_time = clock();
        matrix_get_min_sum_city(graph_matrix, &res);
        curr_time = clock() - curr_time;
        m_sum_time += curr_time;
        ignore(&res);
        
        curr_time = clock();
        list_get_min_sum_city(graph_list, &res);
        curr_time = clock() - curr_time;
        l_sum_time += curr_time;
        ignore(&res);
    }

    m_sum_time = m_sum_time / (CLOCKS_PER_SEC / 1000000LLU);
    m_size = graph_matrix.rows * graph_matrix.cols * sizeof(graph_matrix.data[0]) + sizeof(size_t) * 2;

    l_sum_time = l_sum_time / (CLOCKS_PER_SEC / 1000000LLU);
    l_size = get_list_struct_size(graph_list);
  
    printf("\nКОЛИЧЕСТВО ГОРОДОВ: %zu\n", graph_list.size);
    printf("КОЛИЧЕСТВО ИТЕРАЦИЙ: %d\n", ITERATIONS); 
    puts("\nСРЕДНЕЕ ВРЕМЯ ПОИСКА ГОРОДА С МИН. СУММОЙ ПУТЕЙ ДО ВСЕХ ОСТАЛЬНЫХ");
    printf("С использованием матрицы смежности: %llu мкс\n", m_sum_time);
    printf("С использованием списков ребер: %llu мкс\n", l_sum_time);

    puts("");
    puts("ЗАНИМАЕМЫЙ ОБЪЕМ ПАМЯТИ");
    printf("С использованием матрицы смежности: %zu Б\n", m_size);
    printf("С использованием списков ребер: %zu Б\n", l_size);
}
