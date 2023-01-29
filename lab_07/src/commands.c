#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "avltree.h"
#include "bstree.h"
#include "commands.h"
#include "errors.h"
#include "hashtable.h"
#include "input.h"
#include "treeviz.h"

#ifndef NMAX
#define NMAX 3000
#endif

#define ITERATIONS 20
#define F_COUNT 5
#define PRINT_MSG(n) fprintf(stdout, MSG_COM##n"\n")

static tree_t bstree;
static avltree_t avltree;
static struct hash_table hash_table;
static char file_name[LEN_FILENAME];

static char files[F_COUNT][LEN_FILENAME] = { "./data/500.txt", "./data/1000.txt", "./data/2000.txt", "./data/500_1.txt", "./data/500__1.txt" };
static size_t sizes[F_COUNT] = { 500, 1000, 2000, 500, 500 };

void ignore(void *n)
{
    if (n)
        return;
}

void command_search_results(void)
{
    srand(time(NULL));
    FILE *f = NULL;
    long long unsigned curr_time = 0, sum_time = 0;
    size_t curr_size = 0;
    key_t curr_key = 0;
    tree_t res_node1 = NULL;
    avltree_t res_node2 = NULL;
    double res_comp = 0, max_comp, sum_comp = 0;

    if (input_comp(&max_comp))
    {
        show_err(ERR_DATA);
        return;
    }

    if (bstree != NULL)
        delete_tree(&bstree);
    
    if (hash_table.data != NULL)
        free_hashtable(&hash_table);
    
    if (avltree != NULL)
        avl_delete_tree(&avltree);

    key_t values[ITERATIONS];

    for (size_t i = 0; i < ITERATIONS; i++)
        values[i] = rand() % (20000 - (-20000) + 1) + (-20000);

    for (size_t name_ind = 0; name_ind < F_COUNT; name_ind++)
    {
        f = fopen(files[name_ind], "rt");
        
        printf("%4zu              Время (мкс)      Сравнения         Размер\n", sizes[name_ind]);
        
        // ДДП
        fseek(f, 0, SEEK_SET);
        bstree = fget_bstree(f);
        sum_time = 0;
        sum_comp = 0;

        for (size_t i = 0; i < ITERATIONS; i++)
        {
            null_comparisons();
            curr_key = values[i];
            curr_time = clock();
            res_node1 = search_node(bstree, curr_key);
            curr_time = clock() - curr_time;
            ignore(res_node1);

            sum_comp += get_comparisons();
            sum_time += curr_time;
        }

        sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
        curr_size = bst_get_size() * sizes[name_ind];

        printf("ДДП           : %15.2f %10.2f %15zu\n",
               (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);

        // АВЛ
        fseek(f, 0, SEEK_SET);
        avltree = fget_avltree(f);
        sum_time = 0;
        sum_comp = 0;

        for (size_t i = 0; i < ITERATIONS; i++)
        {
            curr_key = values[i];
            curr_time = clock();
            res_node2 = avl_search_node(avltree, curr_key);
            curr_time = clock() - curr_time;
            ignore(res_node2);

            sum_comp += avl_get_comparisons();
            sum_time += curr_time;
        }

        sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
        curr_size = avl_get_size() * sizes[name_ind];

        printf("АВЛ           : %15.2f %10.2f %15zu\n",
               (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);

        // ХЕШ closed
        hash_table.addr_code = CLOSED;
        fseek(f, 0, SEEK_SET);
        fget_hashtable(f, &hash_table);
        sum_time = 0;
        sum_comp = 0;

        for (size_t i = 0; i < ITERATIONS; i++)
        {
            curr_key = values[i];
            curr_time = clock();
            search_hashtable(&hash_table, curr_key, 0);
            curr_time = clock() - curr_time;

            sum_comp += hash_table.comparisons;
            sum_time += curr_time;
        }

        sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
        curr_size = hash_get_size() * hash_table.size + sizeof(size_t);

        printf("ХЕШ (откр)    : %15.2f %10.2f %15zu\n",
               (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);
        printf("Коллизии: %zu\n", hash_table.collisions);

        res_comp = sum_comp / ITERATIONS;

        // реструктуризация
        if (res_comp - max_comp >= EPS)
        {
            fseek(f, 0, SEEK_SET);
            restructure_hashtable(f, &hash_table);
            sum_time = 0;
            sum_comp = 0;

            for (size_t i = 0; i < ITERATIONS; i++)
            {
                curr_key = values[i];
                curr_time = clock();
                search_hashtable(&hash_table, curr_key, 0);
                curr_time = clock() - curr_time;

                sum_comp += hash_table.comparisons;
                sum_time += curr_time;
            }

            sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
            curr_size = hash_get_size() * hash_table.size + sizeof(size_t) * 4 + sizeof(int) + sizeof(hash_func_pt);

            printf("ХЕШ (re)      : %15.2f %10.2f %15zu\n",
                   (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);
            printf("Коллизии: %zu\n", hash_table.collisions);
        }

        free_hashtable(&hash_table);

        // ХЕШ opened
        fseek(f, 0, SEEK_SET);
        hash_table.addr_code = OPENED;
        fget_hashtable(f, &hash_table);
        sum_time = 0;
        sum_comp = 0;

        for (size_t i = 0; i < ITERATIONS; i++)
        {
            curr_key = values[i];
            curr_time = clock();
            search_hashtable(&hash_table, curr_key, 0);
            curr_time = clock() - curr_time;

            sum_comp += hash_table.comparisons;
            sum_time += curr_time;
        }

        sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
        curr_size = hash_get_size() * hash_table.size + sizeof(size_t) + sizeof(size_t) * 4 + sizeof(int) + sizeof(hash_func_pt);

        printf("ХЕШ (закр)    : %15.2f %10.2f %15zu\n",
               (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);
        printf("Коллизии: %zu\n", hash_table.collisions);

        res_comp = sum_comp / ITERATIONS;

        // реструктуризация
        if (res_comp - max_comp >= EPS)
        {
            fseek(f, 0, SEEK_SET);
            restructure_hashtable(f, &hash_table);
            sum_time = 0;
            sum_comp = 0;

            for (size_t i = 0; i < ITERATIONS; i++)
            {
                curr_key = values[i];
                curr_time = clock();
                search_hashtable(&hash_table, curr_key, 0);
                curr_time = clock() - curr_time;

                sum_comp += hash_table.comparisons;
                sum_time += curr_time;
            }

            sum_time = sum_time / (CLOCKS_PER_SEC / 1000000LLU);
            curr_size = hash_get_size() * hash_table.size + sizeof(size_t) + sizeof(size_t) * 4 + sizeof(int) + sizeof(hash_func_pt);

            printf("ХЕШ (re)      : %15.2f %10.2f %15zu\n",
                   (double) sum_time / ITERATIONS, sum_comp / ITERATIONS, curr_size);
            printf("Коллизии: %zu\n", hash_table.collisions);
        }

        fclose(f);
        delete_tree(&bstree);
        free_hashtable(&hash_table);
        avl_delete_tree(&avltree);

        puts("");
    }
}

void command_show_rules(void)
{
    fprintf(stdout, MSG_RULES);
}

void command_show_menu(void)
{
    fprintf(stdout, "\nТЕКУЩИЙ ФАЙЛ: %s", (strlen(file_name) == 0) ? "Не выбран" : file_name );

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
    /* PRINT_MSG(13); */
    /* PRINT_MSG(14); */
    /* PRINT_MSG(15); */
    PRINT_MSG(0);
}

void reset_filename(void)
{
    file_name[0] = '\0';
}

void command_clear_all(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return;
    }

    reset_filename();
    puts("Выбор файла был успешно сброшен");

    delete_tree(&bstree);
    puts("ДДП было успешно очищено");

    avl_delete_tree(&avltree);
    puts("АВЛ-дерево было успешно очищено");

    free_hashtable(&hash_table);
    puts("Хеш-таблица была успешно очищена");
}

void command_restruct(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return;
    }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL)
    {
        reset_filename();
        show_err(ERR_OPEN);
        return;
    }

    restructure_hashtable(f_curr, &hash_table);

    if (fclose(f_curr) == EOF)
    {
        reset_filename();
        show_err(ERR_CLOSE);
        return;
    }

    puts("Таблица была успешно реструктурирована\n");
}

void command_end(void)
{
    delete_tree(&bstree);
    avl_delete_tree(&avltree);
    free_hashtable(&hash_table);
}

int command_set_filename(void)
{
    if (input_filename(file_name))
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    FILE *f_curr = fopen(file_name, "rt");

    if (f_curr == NULL)
    {
        reset_filename();
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    tree_t tmp_bstree = fget_bstree(f_curr);

    if (tmp_bstree == NULL)
    {
        reset_filename();
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    if (fclose(f_curr) == EOF)
    {
        reset_filename();
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    delete_tree(&tmp_bstree);
    printf("Файл с данными был успешно задан\n");
    
    return EXIT_SUCCESS;
}

int get_addr_code(int *const code)
{
    fprintf(stdout, "Введите код адресации (0 - закр., 1 - откр.): ");

    if ((fscanf(stdin, "%d", code) != READ_COUNT) ||
        ((*code != OPENED) && (*code != CLOSED)))
        return EXIT_FAILURE;

    fgetc(stdin);

    return EXIT_SUCCESS;
}

void command_new_file(void)
{
    if (input_filename(file_name))
    {
        show_err(ERR_DATA);
        return;
    }

    FILE *f = fopen(file_name, "wt");

    if (f == NULL)
    {
        reset_filename();
        show_err(ERR_OPEN);
        return;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        return;
    }
}

void command_set_file(void)
{
    if (command_set_filename()) return;
    
    int addr_code = 0;
    
    if (get_addr_code(&addr_code))
    {
        printf("Ошибка кода\n");
        show_err(ERR_DATA);
        reset_filename();
        return;
    }
    
    if (command_fill_bstree()) return;

    if (command_fill_avltree()) return;
    
    if (command_fill_hashtable(addr_code)) return;
}

void command_switch_hashtable(void)
{
    int new_code = (hash_table.addr_code == OPENED) ? CLOSED : OPENED ;

    if (command_fill_hashtable(new_code)) return;
}

void show_trees(void)
{
    system("dot ./viz/bstree.dot -Tpng > ./viz/bstree.png");
    system("gwenview --title \"Current bstree\" ./viz/bstree.png");
    system("dot ./viz/avltree.dot -Tpng > ./viz/avltree.png");
    system("gwenview --title \"Current avltree\" ./viz/avltree.png");
}

void command_show_trees(void)
{
    if (bstree == NULL) { show_err(ERR_EMPTY); return; }

    if (open_dot("./viz/bstree.dot")) { show_err(ERR_OPEN); return; }

    export_tree_to_dot("bstree", bstree);

    if (close_dot()) { show_err(ERR_CLOSE); return; }

    printf("ДДП было успешно экспортировано в файл bstree.dot\n");

    if (open_dot("./viz/avltree.dot")) { show_err(ERR_OPEN); return; }

    export_avltree_to_dot("avltree", avltree);

    if (close_dot()) { show_err(ERR_CLOSE); return; }

    printf("АВЛ-дерево было успешно экспортировано в файл avltree.dot\n");

    show_trees();
}

void add_to_bstree(const key_t key)
{
    tree_t new_node = create_node(key);
    bstree = insert_node(bstree, new_node);

    if (get_rc())
    {
        fprintf(stdout, "Элемент %d уже присутствует в ДДП\n", key);
        reset_rc();
    }
    else
        fprintf(stdout, "Элемент %d был успешно вставлен в ДДП\n", key);
}

void add_to_avltree(const key_t key)
{
    avltree_t new_node = avl_create_node(key);
    avltree = avl_insert_node(avltree, new_node);

    if (avl_get_rc())
    {
        fprintf(stdout, "Элемент %d уже присутствует в АВЛ-дереве\n", key);
        avl_reset_rc();
    }
    else
        fprintf(stdout, "Элемент %d был успешно вставлен в АВЛ-дерево\n", key);
}

int add_to_hashtable(const key_t key)
{
    int rc = insert_to_hashtable(&hash_table, key);

    if (rc)
    {
        free_hashtable(&hash_table);
        show_err(rc);
        return rc;
    }

    rc = hash_get_rc();

    if (rc)
    {
        hash_reset_rc();
        return rc;
    }

    hash_table.size++;
    fprintf(stdout, "Элемент %d был успешно вставлен в хеш-таблицу\n", key);

    return EXIT_SUCCESS;
}

int add_to_file(const int new_key)
{
    FILE *f = fopen(file_name, "at");

    if (f == NULL)
    {
        show_err(ERR_OPEN);
        return EXIT_FAILURE;
    }

    fprintf(f, "\n%d\n", new_key);

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        return EXIT_FAILURE;
    }

    printf("Элемент %d был успешно добавлен в файл %s\n", new_key, file_name);

    return EXIT_SUCCESS;
}

void command_add_key(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return;
    }
    
    key_t new_key;

    if (input_key(&new_key))
    {
        show_err(ERR_DATA);
        return;
    }

    int rc = add_to_hashtable(new_key);

    if (rc)
    {
        show_err(rc);
        return;
    }

    add_to_file(new_key);
    add_to_bstree(new_key);
    add_to_avltree(new_key);    
}

int command_fill_bstree(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return ERR_FILE;
    }

    FILE *f = fopen(file_name, "rt");

    if (f == NULL)
    {
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    bstree = fget_bstree(f);

    if (bstree == NULL)
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    printf("ДДП было успешно заполнено\n");
    
    return EXIT_SUCCESS;
}

int command_fill_avltree(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return ERR_FILE;
    }

    FILE *f = fopen(file_name, "rt");

    if (f == NULL)
    {
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    avltree = fget_avltree(f);

    if (avltree == NULL)
    {
        show_err(ERR_DATA);
        return ERR_DATA;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    printf("АВЛ-дерево было успешно заполнено\n");
    
    return EXIT_SUCCESS;
}

int command_fill_hashtable(const int addr_code)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return ERR_FILE;
    }

    FILE *f = fopen(file_name, "rt");

    if (f == NULL) 
    {
        show_err(ERR_OPEN);
        return ERR_OPEN;
    }

    hash_table.addr_code = addr_code;
    int rc = fget_hashtable(f, &hash_table);

    if (rc)
    {
        show_err(rc);
        return rc;
    }

    if (fclose(f) == EOF)
    {
        show_err(ERR_CLOSE);
        return ERR_CLOSE;
    }

    printf("Хеш-таблица была успешно заполнена\n");

    return EXIT_SUCCESS;
}

void _search_avl(const key_t key)
{
    avltree_t node = avl_search_node(avltree, key);
    
    if (node == NULL)
    {
        printf("Элемент %d не был найден в АВЛ-дереве\n", key);
        return;
    }
        
    printf("Элемент %d найден в АВЛ-дереве: ", key);
    avl_print_node(node);
    printf("- Кол-во сравнений: %zu\n", avl_get_comparisons());
}

void _search_bst(const key_t key)
{
    tree_t node = search_node(bstree, key);
    
    if (node == NULL)
    {
        printf("\nЭлемент %d не был найден в ДДП\n", key);
        return;
    }
        
    printf("\nЭлемент %d найден в ДДП: ", key);
    print_node(node);
    printf("- Кол-во сравнений: %zu\n", get_comparisons());
}

void _search_hash(const key_t key)
{
    search_hashtable(&hash_table, key, 1);
    printf("- Кол-во сравнений: %zu\n", hash_table.comparisons);
}

void command_search_key(void)
{
    if (strlen(file_name) == 0)
    {
        show_err(ERR_FILE);
        return;
    }

    int key;

    if (input_key(&key))
    {
        show_err(ERR_DATA);
        return;
    }

    _search_bst(key);
    _search_avl(key);
    _search_hash(key);
}

void command_show_hashtable(void)
{
    if (hash_table.size == 0) { show_err(ERR_EMPTY_HASH); return; }

    fprintf(stdout, "Текущая хеш-таблица:\n");
    show_hashtable(&hash_table);
}

void null_data(int *data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
        data[i] = 0;
}


