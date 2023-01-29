#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdio.h>

#include "bstree.h"

#define TRUE 1
#define FALSE 0

#define STEP_DIV 10
#define KOEF 2

#define OPENED 0
#define CLOSED 1

#define READ_COUNT 1

typedef struct hash_table *hash_t;

typedef struct hash_elem *hashtable_t;
typedef int (*hash_func_pt)(hash_t table, const key_t key);

struct hash_table
{
    hashtable_t data;
    size_t size;
    size_t comparisons;
    size_t collisions;
    size_t try_count;
    int addr_code;
    hash_func_pt hash_func;
};

hashtable_t alloc_hashtable(const size_t size);
int fill_hashtable(FILE *const f, hash_t table);
void free_hashtable(hash_t table);
int restructure_hashtable(FILE *const f, hash_t table);
int search_hashtable(hash_t table, const key_t key, const int code);
void show_hashtable(hash_t table);
int insert_to_hashtable(hash_t table, const key_t key);
size_t get_greater_prime(const size_t n);
int hash_get_rc(void);
void hash_reset_rc(void);
size_t hash_get_size(void);

#endif
