#include <stdlib.h>

#include "errors.h"
#include "matrix.h"
#include "limits.h"

void free_matrix(my_matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; i++)
        free(matrix->data[i]);

    free(matrix->data);
    matrix->rows = 0;
    matrix->cols = 0;
}

my_matrix_t alloc_matrix(const size_t rows, const size_t cols)
{
    my_matrix_t matrix = { .rows = rows, .cols = cols };
    matrix.data = calloc(rows, sizeof(size_t *));

    if (matrix.data != NULL)
        for (size_t i = 0; i < rows; i++)
        {
            matrix.data[i] = calloc(cols, sizeof(size_t));

            if (matrix.data[i] == NULL)
                free_matrix(&matrix);
        }

    return matrix;
}

void print_matrix(my_matrix_t matrix)
{
    printf("     ");
    
    for (size_t i = 0; i < matrix.rows; i++)
       printf("%3zu ", i + 1);

    printf("\n");

    for (size_t i = 0; i < matrix.rows; i++)
    {
        printf("%3zu: ", i + 1);

        for (size_t j = 0; j < matrix.cols; j++)
            printf("%3zu ", matrix.data[i][j]);

        puts("");
    }    
}

int fget_graph_matrix(FILE *const f, my_matrix_t *graph_matrix)
{
    long unsigned curr_fpos = ftell(f);
    fseek(f, 0, SEEK_SET);

    size_t num_from, num_to;

    if ((fscanf_size(f, &num_from)) || (fscanf_size(f, &num_to)))
        return ERR_DATA;

    *graph_matrix = alloc_matrix(num_from, num_to);
    
    for (size_t i = 0; i < num_from; i++)
        for (size_t j = 0; j < num_to; j++)
            if (fscanf(f, "%zu", &graph_matrix->data[i][j]) != READ_COUNT)
            {
                free_matrix(graph_matrix);
                return ERR_DATA;
            }

    fseek(f, curr_fpos, SEEK_SET);
    return EXIT_SUCCESS;
}

size_t get_min_dist_vertex(const size_t num_of_v, size_t *dist, bool *check_status)
{
    size_t min = INT_MAX, min_index;

    for (size_t v = 0; v < num_of_v; v++)
        if ((check_status[v] == FALSE) && (dist[v] < min))
        {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}

size_t *get_matrix_dijkstra(my_matrix_t graph_matrix, const size_t source_v)
{
    size_t num_of_v = graph_matrix.rows;
    size_t *dist_from_src = calloc(num_of_v, sizeof(size_t));
    bool *check_status = calloc(num_of_v, sizeof(bool));

    if (dist_from_src == NULL)
        return NULL;

    if (check_status == NULL)
    {
        free(dist_from_src);
        return NULL;
    }

    for (size_t i = 0; i < num_of_v; i++)
    {
        dist_from_src[i] = INT_MAX;
        check_status[i] = FALSE;
    }

    dist_from_src[source_v] = 0;
    //check_status[source_v] = TRUE;
    size_t v;

    for (size_t i = 0; i < num_of_v; i++)
    {
        v = get_min_dist_vertex(num_of_v, dist_from_src, check_status);
        check_status[v] = TRUE;

        for (size_t w = 0; w < num_of_v; w++)
            if ((!check_status[w]) && (graph_matrix.data[v][w] > 0)
                && (dist_from_src[v] != INT_MAX) && (dist_from_src[v] + graph_matrix.data[v][w] < dist_from_src[w]))
                dist_from_src[w] = dist_from_src[v] + graph_matrix.data[v][w]; 
    }

    free(check_status);

    return dist_from_src;
}

size_t get_arr_sum(const size_t size, size_t *arr)
{
    if (arr == NULL)
        return INT_MAX;

    size_t sum = 0;

    for (size_t i = 0; i < size; i++)
        sum += arr[i];

    return sum;
}

int matrix_get_min_sum_city(my_matrix_t matrix, result_t *result)
{
    size_t num_of_v = matrix.rows, min_sum = INT_MAX, min_index = 0;
    size_t *curr_distances = NULL, curr_sum = 0;

    for (size_t curr_v = 0; curr_v < num_of_v; curr_v++)
    {
        curr_distances = get_matrix_dijkstra(matrix, curr_v);

        if (curr_distances == NULL)
            return ERR_MEM;

        curr_sum = get_arr_sum(num_of_v, curr_distances);

        if (curr_sum < min_sum)
        {
            min_sum = curr_sum;
            min_index = curr_v;
        }

        free(curr_distances);
    }

    result->vertex = min_index;
    result->min_sum = min_sum;

    return EXIT_SUCCESS;
}
