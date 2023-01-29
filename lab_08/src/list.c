#include <stdlib.h>

#include "limits.h"
#include "list.h"

struct list_elem
{
    size_t dist;
    size_t vertex;
    list_data_t next;
};

void init_list_elem(list_data_t elem)
{
    elem->dist = 0;
    elem->vertex = 0;
    elem->next = NULL;
}

size_t get_list_struct_size(list_t list)
{
    size_t node_count = 0, res_size = 0;
    struct list_elem *curr;

    for (size_t i = 0; i < list.size; i++)
    {
        node_count = 0;
        curr = list.nodes[i];

        while (curr->next != NULL)
        {
            node_count++;
            curr = curr->next;
        }

        res_size += node_count * sizeof(struct list_elem);
    }
    
    return res_size + list.size * sizeof(struct list_elem *) + sizeof(size_t);
}

list_data_t create_list_elem(const size_t vertex, const size_t dist)
{
    list_data_t new_elem = malloc(sizeof(struct list_elem));

    if (new_elem != NULL)
    {
        init_list_elem(new_elem);
        new_elem->dist = dist;
        new_elem->vertex = vertex;
    }

    return new_elem;
}

void free_list(list_t *list)
{
    if (list->size == 0)
        return;

    list_data_t tmp, curr;

    for (size_t i = 0; i < list->size; i++)
    {
        tmp = NULL;
        curr = list->nodes[i];

        while (curr != NULL)
        {
            tmp = curr->next;
            free(curr);
            curr = tmp;
        }
    }

    list->size = 0;
    list->nodes = NULL;
}

list_data_t *alloc_list_data(const size_t num_of_nodes)
{
    return calloc(num_of_nodes, sizeof(list_data_t *));
}

void print_dot_list(FILE *const fviz, list_t list)
{
    list_data_t curr = NULL;

    for (size_t i = 0; i < list.size; i++)
    {
        curr = list.nodes[i];

        while (curr != NULL)
        {
            fprintf(fviz, "%zu -> %zu [label=\"%zu\"];\n", i + 1, curr->vertex + 1, curr->dist);
            curr = curr->next; 
        }
    }
}

void print_list(list_t list)
{
    list_data_t curr = NULL;

    for (size_t i = 0; i < list.size; i++)
    {
        printf("%zu\n", i + 1);
        curr = list.nodes[i];

        while (curr != NULL)
        {
            printf("-> %zu: %zu\n", curr->vertex + 1, curr->dist);
            curr = curr->next;
        }
    }
}

static size_t get_min_dist_vertex(const size_t num_of_v, size_t *dist, bool *check_status)
{
    size_t min = INT_MAX, min_index = 0;

    for (size_t v = 0; v < num_of_v; v++)
        if ((check_status[v] == FALSE) && (dist[v] < min))
        {
            min = dist[v];
            min_index = v;
        }

    return min_index;
}

size_t get_dist_from_to(list_data_t *data, const size_t v, const size_t w)
{
    list_data_t curr = data[v];

    while (curr != NULL)
    {
        if (curr->vertex == w)
            return curr->dist;
   
        curr = curr->next; 
    }

    return INT_MAX;
}

size_t *get_list_dijkstra(list_t graph_list, const size_t source_v)
{
    size_t num_of_v = graph_list.size;
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
    size_t v;

    for (size_t i = 0; i < num_of_v; i++)
    {
        v = get_min_dist_vertex(num_of_v, dist_from_src, check_status);
        check_status[v] = TRUE;

        for (size_t w = 0; w < num_of_v; w++)
            if ((!check_status[w]) && (get_dist_from_to(graph_list.nodes, v, w) > 0)
                && (dist_from_src[v] != INT_MAX) && (dist_from_src[v] + get_dist_from_to(graph_list.nodes, v, w) < dist_from_src[w]))
                dist_from_src[w] = dist_from_src[v] + get_dist_from_to(graph_list.nodes, v, w); 
    }

    free(check_status);

    return dist_from_src;
}

static size_t get_arr_sum(const size_t size, size_t *arr)
{
    if (arr == NULL)
        return INT_MAX;

    size_t sum = 0;

    for (size_t i = 0; i < size; i++)
        sum += arr[i];

    return sum;
}

int list_get_min_sum_city(list_t list, result_t *result)
{
    size_t num_of_v = list.size, min_sum = INT_MAX, min_index;
    size_t *curr_distances = NULL, curr_sum = 0;

    for (size_t curr_v = 0; curr_v < num_of_v; curr_v++)
    {
        curr_distances = get_list_dijkstra(list, curr_v);

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

int fget_graph_list(FILE *const f, list_t *graph_list)
{
    long unsigned curr_fpos = ftell(f);
    fseek(f, 0, SEEK_SET);

    size_t num_of_nodes = 0;

    if (fscanf_size(f, &num_of_nodes))
        return ERR_DATA;

    graph_list->nodes = alloc_list_data(num_of_nodes);
    graph_list->size = num_of_nodes;

    if (graph_list->nodes == NULL)
        return ERR_MEM;

    size_t city_to, city_from, curr_dist;
    struct list_elem *new_elem, *curr;

    // По условию для каждой пары есть соединяющий путь
    for (size_t i = 0; i < (num_of_nodes - 1) * num_of_nodes; i++)
    {
        if (fscanf(f, "%zu", &city_from) != READ_COUNT)
            return ERR_DATA;

        if (fscanf(f, "%zu", &city_to) != READ_COUNT)
            return ERR_DATA;

        if (fscanf(f, "%zu", &curr_dist) != READ_COUNT)
            return ERR_DATA;

        if (curr_dist == 0)
            return ERR_DATA;
    
        new_elem = create_list_elem(city_to - 1, curr_dist);

        if (new_elem == NULL)
            return ERR_MEM;
       
        curr = graph_list->nodes[city_from - 1]; 
    
        if (curr != NULL)
        {
            while (curr->next != NULL)
                curr = curr->next;

            curr->next = new_elem;
        }
        else
            graph_list->nodes[city_from - 1] = new_elem;
    }

    fseek(f, curr_fpos, SEEK_SET);
    return EXIT_SUCCESS;
}
