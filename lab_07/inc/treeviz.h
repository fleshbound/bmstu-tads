#ifndef TREEVIZ_H
#define TREEVIZ_H

#include "bstree.h"
#include "avltree.h"

int open_dot(const char *filename);
int close_dot(void);
void export_tree_to_dot(const char *tree_name, tree_t tree);
void export_node_to_dot(const char *tree_name, tree_t node);
void export_avltree_to_dot(const char *tree_name, avltree_t tree);
void export_avlnode_to_dot(const char *tree_name, avltree_t tree);

#endif
