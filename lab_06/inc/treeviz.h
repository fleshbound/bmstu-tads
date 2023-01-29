#ifndef TREEVIZ_H
#define TREEVIZ_H

#include "bstree.h"

int open_dot(const char *filename);
int close_dot(void);
void export_tree_to_dot(const char *tree_name, tree_t tree);
void export_node_to_dot(const char *tree_name, tree_t node);

#endif
