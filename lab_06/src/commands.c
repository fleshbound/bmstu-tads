#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "bstree.h"
#include "commands.h"
#include "errors.h"
#include "input.h"
#include "tree.h"
#include "treeviz.h"

#ifndef NMAX
#define NMAX 3000
#endif

#define ITERATIONS 200

static tree_t tree;

void command_show_rules(void)
{
    fprintf(stdout, MSG_RULES);
}

void command_show_menu(void)
{
    fprintf(stdout, MSG_COM1"\n");
    fprintf(stdout, MSG_COM2"\n");
    fprintf(stdout, MSG_COM3"\n");
    fprintf(stdout, MSG_COM4"\n");
    fprintf(stdout, MSG_COM5"\n");
    fprintf(stdout, MSG_COM6"\n");
    fprintf(stdout, MSG_COM7"\n");
    fprintf(stdout, MSG_COM8"\n");
    fprintf(stdout, MSG_COM9"\n");
    fprintf(stdout, MSG_COM10"\n");
    fprintf(stdout, MSG_COM11"\n");
    fprintf(stdout, MSG_COM12"\n");
    fprintf(stdout, MSG_COM13"\n");
    fprintf(stdout, MSG_COM14"\n");
    fprintf(stdout, MSG_COM15"\n");
    fprintf(stdout, MSG_COM0"\n");
}

void command_fill_tree(void)
{
    char file_name[LEN_FILENAME] = "";

    if (input_filename(file_name)) { show_err(ERR_DATA); return; }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL) { show_err(ERR_OPEN); return; }

    tree = fget_tree(f_curr);

    if (tree == NULL) { show_err(ERR_DATA); return; }

    if (fclose(f_curr) == EOF) { show_err(ERR_CLOSE); return; }

    printf("Дерево было успешно заполнено\n");
}

void command_export_tree(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    if (open_dot("./viz/tree.dot")) { show_err(ERR_OPEN); return; }

    export_tree_to_dot("current_tree", tree);

    if (close_dot()) { show_err(ERR_CLOSE); return; }

    printf("Дерево было успешно экспортировано в файл tree.dot\n");
}

int input_code(char *msg, int *const code)
{
    printf("%s", msg);
    
    if (fscanf(stdin, "%d", code) <= 0)
        return EXIT_FAILURE;
    fgetc(stdin);

    if ((*code != NEW_TREE) && (*code != EXIST_TREE))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void command_add_node(void)
{
    int new_key = 0;
    char file_name[LEN_FILENAME] = "";

    int code = 0;
    
    if (input_code("1 - создать новое дерево, 2 - добавить в сущ-ее: ", &code))
    {
        show_err(ERR_DATA);
        return;       
    }

    if (input_filename(file_name)) { show_err(ERR_DATA); return; }
  
    if (finput_node(stdin, &new_key, VERBOSE)) { show_err(ERR_DATA); return; }
    
    FILE *f = NULL;

    if (code == EXIST_TREE) 
    {
        f = fopen(file_name, "rt"); 

        if (f == NULL) { show_err(ERR_OPEN); return; }

        tree = fget_tree(f);

        if (tree == NULL) { show_err(ERR_DATA); return; }
        
        if (fclose(f) == EOF) { show_err(ERR_CLOSE); return; }     

        if (search_node(tree, new_key) != NULL)
        {
            printf("Узел уже присутствует в дереве\n");
            return;
        }
    } 
    else if (code == NEW_TREE)
    {
        f = fopen(file_name, "wt");
        
        if (f == NULL) { show_err(ERR_OPEN); return; }

        delete_tree(&tree);

        fclose(f);
    }

    tree_t new_node = create_node(new_key);

    // tree insert time
    long long unsigned tree_insert_time = clock();
    tree = insert_node(tree, new_node);
    tree_insert_time = clock() - tree_insert_time;
    tree_insert_time = tree_insert_time / (CLOCKS_PER_SEC / 1000000LLU);
    printf("Вставка в дерево: %10llu\n", tree_insert_time);

    f = fopen(file_name, "at");

    // file insert time
    long long unsigned file_insert_time = clock();
    fprintf(f, " %d ", new_key);
    file_insert_time = clock() - file_insert_time;
    file_insert_time = file_insert_time / (CLOCKS_PER_SEC / 1000000LLU);
    printf("Вставка в файл: %10llu\n", file_insert_time);
    
    if (fclose(f) == EOF) { show_err(ERR_CLOSE); return; }

    printf("Эффективность относительно вставки в файл: %.3f%%\n",
            (((double) file_insert_time - (double) tree_insert_time)
             / file_insert_time) * 100);

    printf("Узел был успешно вставлен\n");
}

void command_del_node(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    int del_key = 0;

    if (input_node(&del_key)) { show_err(ERR_DATA); return; }

    tree = delete_node(tree, del_key);

    printf("Узел был успешно удален\n");
}

void show_node(void)
{
    int rc = system("dot ./viz/search.dot -Tpng > ./viz/search.png");
    printf("%d\n", rc);
    rc = system("gwenview --title \"Search result\" ./viz/search.png");
    printf("%d\n", rc);
}

void command_tree_search(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    int search_key = 0;

    if (input_node(&search_key)) { show_err(ERR_DATA); return; }

    tree_t node = search_node(tree, search_key);
    
    if (node == NULL)
    {
        printf("Искомого узла нет в записанном дереве\n");
        return;
    }

    print_node(node, stdout);
    if (open_dot("./viz/search.dot")) { show_err(ERR_OPEN); return; };
    export_node_to_dot("search_node", node);
    close_dot();
    show_node();
}

void command_tree_lookup(const int code)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    FILE *log = fopen("./out/log.txt", "wt");
    long long unsigned curr_time = 0, lookup_time_1 = 0,
         lookup_time_2 = 0, lookup_time_3 = 0;

    size_t limit = ITERATIONS;
    if (code == VERBOSE) limit = 1;

    for (size_t i = 0; i < limit; i++)
    {
        if (code == VERBOSE) 
        {
            int height = get_height(tree);
            printf("Высота дерева: %d\n", height);

            printf("Префиксный обход:\n");
            curr_time = clock();
            apply_tree(PRE, tree, print_node_lookup, stdout);
            curr_time = clock() - curr_time;
            printf("\n");
        }
        else 
        {
            curr_time = clock();
            apply_tree(PRE, tree, print_node, log);
            curr_time = clock() - curr_time;
        }
        lookup_time_1 += curr_time / (CLOCKS_PER_SEC / 1000000LLU);

        if (code == VERBOSE) 
        {
            printf("Инфиксный обход:\n");
            curr_time = clock();
            apply_tree(IN, tree, print_node_lookup, stdout);
            curr_time = clock() - curr_time;
            printf("\n");
        }
        else 
        {
            curr_time = clock();
            apply_tree(IN, tree, print_node, log);
            curr_time = clock() - curr_time;
        }
        lookup_time_2 += curr_time / (CLOCKS_PER_SEC / 1000000LLU);

        if (code == VERBOSE) 
        {
            printf("Постфиксный обход:\n");
            curr_time = clock();
            apply_tree(POST, tree, print_node_lookup, stdout);
            curr_time = clock() - curr_time;
            printf("\n");
        }
        else 
        {
            curr_time = clock();
            apply_tree(POST, tree, print_node, log);
            curr_time = clock() - curr_time;
        }
        lookup_time_3 += curr_time / (CLOCKS_PER_SEC / 1000000LLU);
    }

    fclose(log);

    if (code == QUIET)
    {
        printf("Время префиксного: %llu (мкс)\n", lookup_time_1 / ITERATIONS);
        printf("Время инфиксного: %llu (мкс)\n", lookup_time_2 / ITERATIONS);
        printf("Время постфиксного: %llu (мкс)\n", lookup_time_3 / ITERATIONS);
    }
}

void null_data(int *data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
        data[i] = 0;
}

void command_tree_sort(void)
{
    char file_name[LEN_FILENAME] = "";

    if (input_filename(file_name)) { show_err(ERR_DATA); return; }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL) { show_err(ERR_OPEN); return; }
    
    long long unsigned sort_time = 0, sort_time2 = 0, curr_time = 0, curr_time2 = 0;
    
    curr_time = clock();
    tree = fget_tree(f_curr);
    curr_time = clock() - curr_time;
    fclose(f_curr);
    sort_time2 += curr_time;

    if (tree == NULL) { show_err(ERR_DATA); return; }

    size_t count = 0;
    get_all_count(tree, &count);
    int *data = calloc(count, sizeof(int));

    if (data == NULL) { show_err(ERR_MEM); return; }

    curr_time2 = clock();
    int i = 0;
    store_sorted(tree, data, &i);
    curr_time2 = clock() - curr_time2 + curr_time;
    sort_time += curr_time2;

    for (size_t j = 1; j < ITERATIONS; j++)
    {
        f_curr = fopen(file_name, "rt");
        null_data(data, count);
        curr_time = clock();
        tree = fget_tree(f_curr);
        curr_time = clock() - curr_time;
        i = 0;
        sort_time2 += curr_time;
        curr_time2 = clock();
        store_sorted(tree, data, &i);
        curr_time2 = clock() - curr_time2 + curr_time;
        sort_time += curr_time2;
        fclose(f_curr);
    }

    sort_time = sort_time / (ITERATIONS * CLOCKS_PER_SEC / 1000000LLU);
    sort_time2 = sort_time2 / (ITERATIONS * CLOCKS_PER_SEC / 1000000LLU);

    int height = get_height(tree);
    printf("Высота дерева: %d\n", height);
    printf("Результат сортировки: Время -- %llu мкс (запись -- %llu мкс)\n",
            sort_time, sort_time2);

    for (size_t i = 0; (count < 100) && (i < count) ; i++)
        printf("%d%s", data[i], (i == count - 1) ? "\n" : " ");

    free(data);
    delete_tree(&tree);
}

size_t get_file_count(FILE *const f)
{
    int tmp = 0;
    size_t count = 0;

    while (fscanf(f, "%d", &tmp) == READ_COUNT)
        count++;

    return count;
}

int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void command_mysort(void)
{
    char file_name[LEN_FILENAME] = "";

    if (input_filename(file_name)) { show_err(ERR_DATA); return; }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL) { show_err(ERR_OPEN); return; }
    
    size_t count = get_file_count(f_curr);

    fclose(f_curr);

    long long unsigned curr_time = 0, sort_time = 0;

    curr_time = clock();
    int *data = calloc(count, sizeof(int));
    
    if (data == NULL) { show_err(ERR_MEM); return; }

    f_curr = fopen(file_name, "rt");

    int i = 0;
    while (fscanf(f_curr, "%d", &data[i]) == READ_COUNT)
        i++; 

    fclose(f_curr);
    curr_time = clock() - curr_time;
    sort_time += curr_time;

    for (size_t j = 0; j < ITERATIONS; j++)
    {
        curr_time = clock();
        qsort(data, count, sizeof(int), cmp_int);
        curr_time = clock() - curr_time;
        
        sort_time += curr_time;

        f_curr = fopen(file_name, "rt");
        i = 0;
        curr_time = clock();
        
        while ((j != ITERATIONS - 1) && (fscanf(f_curr, "%d", &data[i]) == READ_COUNT))
            i++; 
        
        curr_time = clock() - curr_time;

        sort_time += curr_time;

        fclose(f_curr);
    }

    sort_time = sort_time / (ITERATIONS * CLOCKS_PER_SEC / 1000000LLU);
    
    printf("Результат сортировки: Время -- %llu мкс\n", sort_time);

    for (size_t i = 0; (count < 100) && (i < count); i++)
        printf("%d%s", data[i], (i == count - 1) ? "\n" : " ");

    free(data);
    delete_tree(&tree);
}


void command_clear_tree(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    delete_tree(&tree);
    printf("Дерево было успешно очищено\n");
}

void command_nodes_count(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    tree_t tmp = tree;
    int i = 0;
    size_t count = -1LLU;

    while (count != 0)
    {
        count = get_nodes_count(tmp, i);
        if (count != 0)
            printf("Уровень %d: %zu узл.\n", i++, count);
    }
}

void command_show_tree(void)
{
    int rc = system("dot ./viz/tree.dot -Tpng > ./viz/tree.png");
    printf("%d\n", rc);
    rc = system("gwenview --title \"Current tree\" ./viz/tree.png");
    printf("%d\n", rc);
}

void command_get_height(void)
{
    if (tree == NULL) { show_err(ERR_EMPTY); return; }

    int h = get_height(tree);
    printf("Высота текущего дерева: %d\n", h);
}

void command_end(void)
{
    if (tree != NULL)
        command_clear_tree();

    system("rm -f ./viz/*");
}
