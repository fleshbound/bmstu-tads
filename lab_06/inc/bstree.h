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
tree_t delete_node(tree_t tree, const key_t key);
void delete_tree(tree_t *tree);
tree_t search_node(tree_t tree, const key_t key);
void apply_tree(const int code, tree_t tree, void (*f)(tree_t, void*), void *arg);
void node_to_dot(tree_t node, void *param);
void store_sorted(tree_t tree, int *data, int *const i);
void print_node(tree_t node, void *param);
size_t get_nodes_count(tree_t tree, int level);
void get_all_count(tree_t tree, size_t *const count);
int get_height(tree_t tree);
void print_node_lookup(tree_t node, void *param);

#endif
