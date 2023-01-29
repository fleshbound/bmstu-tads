#include <stdio.h>
#include <stdlib.h>

#include "viz.h"

static FILE *fviz;

int open_dot(const char *filename)
{
    fviz = fopen(filename, "wt");
    
    if (fviz == NULL)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int close_dot(void)
{
    if (fclose(fviz) == EOF)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void matrix_to_dot(char *const graph_name, my_matrix_t matrix)
{
    if (fviz == NULL)
        return;

    fprintf(fviz, "digraph %s {\n", graph_name);

    for (size_t i = 0; i < matrix.rows; i++)
        for (size_t j = 0; j < matrix.cols; j++)
            if (matrix.data[i][j] != 0)
                fprintf(fviz, "%zu -> %zu [label=\"%zu\"];\n",
                        i + 1, j + 1, matrix.data[i][j]);
    
    fprintf(fviz, "}\n");
}

void list_to_dot(char *const graph_name, list_t list)
{
    if (fviz == NULL)
        return;

    fprintf(fviz, "digraph %s {\n", graph_name);
    print_dot_list(fviz, list);
    fprintf(fviz, "}\n");
}
