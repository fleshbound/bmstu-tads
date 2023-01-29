#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdio.h>

#include "bstree.h"

#define PRE 0
#define IN 1
#define POST 2

typedef struct avltree_node* avltree_t;

avltree_t avl_create_node(const key_t key);
avltree_t avl_insert_node(avltree_t tree, avltree_t node);
void avl_delete_tree(avltree_t *tree);
avltree_t avl_search_node(avltree_t tree, const key_t key);
void avl_apply_tree(const int code, avltree_t tree, void (*f)(avltree_t, void*), void *arg);
void avl_node_to_dot(avltree_t node, void *param);
void avl_print_node(avltree_t node);
size_t avl_get_comparisons(void);
void avl_reset_rc(void);
int avl_get_rc(void);
size_t avl_get_size(void);

#endif
