#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "errors.h"
#include "input.h"
#include "treeviz.h"

void clear_str(char *const str)
{
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\n')
            str[i] = '\0';
}

int input_filename(char *const name)
{
    printf("Введите имя файла: ");
    
    if (fgets(name, LEN_FILENAME, stdin) == NULL)
        return EXIT_FAILURE;

    clear_str(name);

    return EXIT_SUCCESS;
}

int input_command(void)
{
    fprintf(stdout, MSG_COM_GET);
    char tmp_s[LEN_COM] = "";

    if (fgets(tmp_s, LEN_COM, stdin) == NULL)
        return ERR_COM_IND;

    clear_str(tmp_s);
    if (strlen(tmp_s) == 0)
    {
        show_err(ERR_EMPTYSTR);
        return ERR_COM_IND;
    }

    char *ptr;
    int tmp = strtol(tmp_s, &ptr, 10);

    if ((tmp == 0) && (strlen(ptr) != 0))
    {
        show_err(ERR_DATA);
        return ERR_COM_IND;        
    }

    if ((tmp < MIN_COM_IND) || (tmp > MAX_COM_IND))
    {
        show_err(ERR_DATA);
        tmp = ERR_COM_IND;
    }

    return tmp;
}

int finput_key(FILE *const f, int *const new_key, const int code)
{
    if (code == VERBOSE) fprintf(stdout, "Введите элемент: ");

    if (fscanf(f, "%d", new_key) != READ_COUNT)
        return EXIT_FAILURE;

    fgetc(f);

    return EXIT_SUCCESS;
}

int input_key(int *new_key)
{
    return finput_key(stdin, new_key, VERBOSE);
}

tree_t fget_bstree(FILE *const f)
{
    int key = 0, rc = finput_key(f, &key, QUIET);
    tree_t node = NULL, tree = NULL;

    while (rc == EXIT_SUCCESS)
    {
        /* puts("hi node"); */
        node = create_node(key);
        tree = insert_node(tree, node);
        rc = finput_key(f, &key, QUIET);
    }

    return tree;
}

avltree_t fget_avltree(FILE *const f)
{
    int key = 0, rc = finput_key(f, &key, QUIET);
    avltree_t node = NULL, tree = NULL;

    while (rc == EXIT_SUCCESS)
    {
        node = avl_create_node(key);
        tree = avl_insert_node(tree, node);
        rc = finput_key(f, &key, QUIET);
        
        /* open_dot("./viz/avltree.dot"); */
        /* export_avltree_to_dot("avltree", tree); */
        /* close_dot(); */
        /* system("dot ./viz/avltree.dot -Tpng > ./viz/avltree.png"); */
        /* system("gwenview --title \"Current avltree\" ./viz/avltree.png"); */
    }

    return tree;
}

int fget_hashtable(FILE *const f, hash_t table)
{
    key_t key;

    if (finput_key(f, &key, QUIET))
        return ERR_DATA;

    table->size = get_greater_prime((size_t) (get_file_count(f) * KOEF));

    if (table->size == 0)
        return ERR_DATA;

    table->data = alloc_hashtable(table->size);

    if (table->data == NULL)
        return ERR_MEM;

    int rc = fill_hashtable(f, table);
    
    if (rc)
    {
        puts("Вставка в таблицу принудительно завершена");
        return rc;
    }

    return EXIT_SUCCESS;
}

int input_comp(double *const comp)
{
    printf("Введите макс. число сравнений: ");

    if (fscanf(stdin, "%le", comp) != READ_COUNT)
        return EXIT_FAILURE;

    if (*comp <= 1)
        return EXIT_FAILURE;

    fgetc(stdin);

    return EXIT_SUCCESS;
}

size_t get_file_count(FILE *const f)
{
    long int curr_pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    int tmp = 0;
    size_t count = 0;

    while (fscanf(f, "%d", &tmp) == READ_COUNT)
        count++;

    fseek(f, curr_pos, SEEK_SET);

    return count;
}
