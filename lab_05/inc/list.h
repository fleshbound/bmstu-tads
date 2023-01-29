#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "presets.h"
#include "errors.h"

#define LISTS_EQUAL 0
#define LISTS_DIFFER 1
#define OBJECT_EQUAL 0
#define OBJECTS_DIFFER 1

#define MAX_FREED_NODES_AMOUNT 1000

typedef struct list_queue list_queue_t;

typedef struct freed_nodes
{
    void *addresses[1000];
    int size;
} freed_nodes_t;

list_queue_t *new_list_queue(void);
void free_list_queue(list_queue_t *list_queue);
int list_queue_insert(list_queue_t *list_queue, int value, ssize_t index);
int list_queue_push(list_queue_t *list_queue, int value);
int list_queue_push_address(list_queue_t *list_queue, int value, freed_nodes_t *freed_nodes);
int list_queue_pop(list_queue_t *list_queue, int *value);
int list_queue_pop_address(list_queue_t *list_queue, int *value, freed_nodes_t *freed_nodes);
int list_queue_contains(const list_queue_t *list_queue, int value);
size_t get_list_queue_size(const list_queue_t *list_queue);
void freed_nodes_print(freed_nodes_t *freed_nodes);
void list_queue_print(list_queue_t *list_queue);
size_t get_list_queue_size_bytes(int elements_amount);

#endif  // __LIST_H__
