#ifndef BSTREE_H
#define BSTREE_H

#include <stdio.h>

#define PRE 0
#define IN 1
#define POST 2

typedef struct tree_node* tree_t;
typedef int key_t;

tree_t create_node(const key_t key);
tree_t insert_node(tree_t tree, tree_t node);
void delete_tree(tree_t *tree);
tree_t search_node(tree_t tree, const key_t key);
void apply_tree(const int code, tree_t tree, void (*f)(tree_t, void*), void *arg);
void node_to_dot(tree_t node, void *param);
void print_node(tree_t node);
size_t get_comparisons(void);
void reset_rc(void);
int get_rc(void);
void null_comparisons(void);
size_t bst_get_size(void);

#endif
