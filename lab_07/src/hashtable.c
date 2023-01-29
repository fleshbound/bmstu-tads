#include <stdlib.h>
#include <math.h>

#include "commands.h"
#include "errors.h"
#include "hashtable.h"

struct hash_elem
{
    int not_empty;
    key_t key;
    hashtable_t next;
};

static int _rc;

size_t hash_get_size(void)
{
    return sizeof(struct hash_elem); 
}

void init_hash_elem(hashtable_t elem)
{
    elem->not_empty = FALSE;
    elem->key = 0;
    elem->next = NULL;
}

hashtable_t create_hashelem(const key_t key)
{
    hashtable_t new_elem = malloc(sizeof(struct hash_elem));
    
    if (new_elem != NULL)
    {
       init_hash_elem(new_elem);
       new_elem->key = key;
       new_elem->not_empty = TRUE;
    }

    return new_elem;
}

hashtable_t alloc_hashtable(const size_t size)
{
    hashtable_t new_table = malloc(sizeof(struct hash_elem) * size);
    
    if (new_table != NULL)
        for (size_t i = 0; i < size; i++)
            init_hash_elem(&new_table[i]);

    return new_table;
}

void free_hashtable(hash_t table)
{
    hashtable_t tmp, curr_next;

    for (size_t i = 0; i < table->size; i++)
    {
        curr_next = table->data[i].next;

        while (curr_next != NULL)
        {
            tmp = curr_next->next;
            free(curr_next);
            curr_next = tmp;
        }
    }

    table->size = 0;
    free(table->data);
    table->data = NULL;
}

int is_prime(const size_t n) // assuming n >= 1
{
    if ((n % 2 == 0) || (n % 3 == 0) || (n == 1))
        return FALSE;

    size_t root = (size_t) sqrt(n);

    for (size_t i = 5; i <= root; i +=6)
        if (n % i == 0)
           return FALSE;

    for (size_t i = 7; i <= root; i += 6)
        if (n % i == 0)
           return FALSE;

    return TRUE;
}

size_t get_greater_prime(const size_t n)
{
    size_t x = n;

    while (!is_prime(x))
        x++;

    return x;
}

int get_digit_sum(const int n)
{
    int tmp = abs(n), sum = 0;

    while (tmp > 0)
    {
        sum += tmp % 10;
        tmp = tmp / 10;
    }

    return sum;
}

int _step_func(hash_t table, const key_t key)
{
    if (table->size < 100)
        return 8 - key % 8;
    
    return table->size - 2 - key % (table->size - 2);
}

int closed_hash_func(hash_t table, const key_t key)
{
    return (abs(key) % table->size + (table->try_count) * (table->try_count)) % table->size;
}

int opened_hash_func(hash_t table, const key_t key)
{
    return abs(key) % table->size;
}

int get_hash(hash_t table, const key_t key)
{
    return table->hash_func(table, key);
}

int opened_insert(hash_t table, const key_t key)
{
    int hash = get_hash(table, key);
    hashtable_t new_elem = create_hashelem(key), colis_elem;

    if (new_elem == NULL)
        return ERR_MEM;

    if ((table->data[hash].not_empty) && (key != table->data[hash].key))
    {
        colis_elem = &(table->data[hash]);

        while ((colis_elem->next != NULL) && (colis_elem->key != key))
            colis_elem = colis_elem->next;

        if (colis_elem->key == key)
        {
            free(new_elem);
            _rc = ERR_EXISTING_KEY;
            return EXIT_SUCCESS;
        }

        colis_elem->next = new_elem;
        table->collisions++;
    }
    else if (key != table->data[hash].key)
    {
        table->data[hash] = *new_elem;
        table->data[hash].not_empty = TRUE;
        free(new_elem);
    }
    else
        _rc = ERR_EXISTING_KEY;

    return EXIT_SUCCESS;
}

int closed_insert(hash_t table, const key_t key)
{
    table->try_count = 0;   
    int hash = get_hash(table, key);
    int i = hash;

    if ((table->data[i].not_empty) && (table->data[i].key == key))
    {
        _rc = ERR_EXISTING_KEY;
        return EXIT_SUCCESS;
    }

    /* printf("[%d, s=%zu] hash = %d\n", key, table->size, i); */

    if (table->data[i].not_empty)
    {

        while (((size_t) i < table->size) && (table->data[i].not_empty))
        {
            table->try_count++;
            /* printf("newhash: %d\n", i); */

            if (table->data[i].key == key)
            {
                _rc = ERR_EXISTING_KEY;
                return EXIT_SUCCESS;
            }
            
            table->collisions++;
            
            i = table->hash_func(table, key);
            /* printf("-> new hash = %d\n", i); */
        }
    }
 
    if ((size_t) i >= table->size)
        return ERR_FULL_HASH;

    table->data[i].not_empty = TRUE;
    table->data[i].key = key;

    return EXIT_SUCCESS;
}

int hash_get_rc(void)
{
    return _rc;
}

void hash_reset_rc(void)
{
    _rc = EXIT_SUCCESS;
}

int insert_to_hashtable(hash_t table, const key_t key)
{
    _rc = EXIT_SUCCESS;
    int rc = EXIT_SUCCESS;
        
    if (table->addr_code == OPENED)        
        rc = opened_insert(table, key);
    else if (table->addr_code == CLOSED)
        rc = closed_insert(table, key);

    return rc;
}

int fill_hashtable(FILE *const f, hash_t table)
{
    table->collisions = 0;
    table->comparisons = 0;
    table->try_count = 0;
   
    if (table->addr_code == OPENED)
        table->hash_func = opened_hash_func;
    else if (table->addr_code == CLOSED)
        table->hash_func = closed_hash_func;

    long int curr_pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    key_t curr_key = 0;
    int rc = EXIT_SUCCESS;

    /* printf("%d\n", fscanf(f, "%d", &curr_key)); */

    while (fscanf(f, "%d", &curr_key) == READ_COUNT)
    {
        rc = insert_to_hashtable(table, curr_key);
        
        if (rc)
            return rc;
    }

    fseek(f, curr_pos, SEEK_SET);

    return EXIT_SUCCESS;
}

int restructure_hashtable(FILE *const f, hash_t table)
{
    size_t size = get_greater_prime((size_t) (table->size * KOEF));
    free_hashtable(table);
    table->size = size;
    table->data = alloc_hashtable(table->size);
    table->comparisons = 0;

    if (table->data == NULL)
        return ERR_MEM;

    return fill_hashtable(f, table);
}

int search_hashtable(hash_t table, const key_t key, const int code)
{
    if (table->addr_code == OPENED)
        table->hash_func = opened_hash_func;
    else if (table->addr_code == CLOSED)
        table->hash_func = closed_hash_func;

    table->comparisons = 0;
    table->try_count = 0;
    int curr_hash = table->hash_func(table, key);
    int i = curr_hash;
    hashtable_t curr;

    if ((table->addr_code == OPENED) &&
        (table->data[curr_hash].not_empty))
    {
        curr = &(table->data[curr_hash]);

        while (curr != NULL)
        {
            table->comparisons++;
            
            if (curr->key == key)
            {
                if (code == 1)
                    printf("Элемент %d найден в хеш-таблице, хеш: %d\n", key, curr_hash);
                return EXIT_SUCCESS;
            }
           
            curr = curr->next;
        }
    }
    /* printf("[%d, s=%zu] hash = %d\n", key, table->size, i); */
    
    if ((table->addr_code == CLOSED) &&
        (table->data[i].not_empty))
    {
        table->comparisons++;

        while (((size_t) i < table->size) && (table->data[i].not_empty))
        {
            table->try_count++;
            /* printf("newhash: %d\n", i); */

            if (table->data[i].key == key)
            {
                if (code == 1)
                    printf("Элемент %d найден в хеш-таблице, хеш: %d\n", key, i);
                return EXIT_SUCCESS;
            }
            
            table->comparisons++;

            i = table->hash_func(table, key);
            /* printf("-> new hash = %d\n", i); */
        }
    }
    
    if (!table->data[i].not_empty)
        table->comparisons++;

    if (code == 1)
        printf("Элемент %d не был найден в хеш-таблице\n", key);

    return EXIT_FAILURE;
}

void print_addr(const int code)
{
    printf("Тип адресации: %s\n", (code == OPENED) ? "ЗАКРЫТАЯ" : "ОТКРЫТАЯ");
}

void print_subhashelem(hashtable_t elem)
{
    printf("|          | %-8d |\n", elem->key);
}

void print_hashelem(const size_t index, struct hash_elem elem)
{
    printf("| %-8zu | %-8d |\n", index, elem.key);
    
    hashtable_t next = elem.next;

    while (next != NULL)
    {
        print_subhashelem(next);
        next = next->next;
    }
}

void show_hashtable(hash_t table)
{
    print_addr(table->addr_code);
    printf("|----------|----------|\n");
    printf("| Хеш      | Значение |\n");

    for (size_t i = 0; i < table->size; i++)
        if (table->data[i].not_empty)
        {
            printf("|----------|----------|\n");
            print_hashelem(i, table->data[i]);
        }

    printf("|----------|----------|\n");
    printf("\nРазмер таблицы: %zu\n", table->size);
    printf("Количество КОЛЛИЗИЙ: %zu\n", table->collisions);
}
