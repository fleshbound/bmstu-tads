#include <stdio.h>
#include <stdlib.h>
#include "treeviz.h"

static FILE *fviz;

int open_dot(const char *filename)
{
    fviz = fopen(filename, "wt");
    
    if (fviz == NULL)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int close_dot(void)
{
    if (fclose(fviz) == EOF)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void export_tree_to_dot(const char *tree_name, tree_t tree)
{
    if (fviz != NULL)
    {
        fprintf(fviz, "digraph %s {\n", tree_name);
        apply_tree(PRE, tree, node_to_dot, fviz);
        fprintf(fviz, "}\n");
    }
}

void export_node_to_dot(const char *tree_name, tree_t node)
{
    if (node != NULL)
    {
        fprintf(fviz, "digraph %s {\n", tree_name);
        node_to_dot(node, fviz);
        fprintf(fviz, "}\n");
    }
}

void export_avltree_to_dot(const char *tree_name, avltree_t tree)
{
    if (fviz != NULL)
    {
        fprintf(fviz, "digraph %s {\n", tree_name);
        avl_apply_tree(PRE, tree, avl_node_to_dot, fviz);
        fprintf(fviz, "}\n");
    }
}

void export_avlnode_to_dot(const char *tree_name, avltree_t node)
{
    if (node != NULL)
    {
        fprintf(fviz, "digraph %s {\n", tree_name);
        avl_node_to_dot(node, fviz);
        fprintf(fviz, "}\n");
    }
}
