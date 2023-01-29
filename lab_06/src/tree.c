#include <stdlib.h>
#include "input.h"
#include "tree.h"

tree_t fget_tree(FILE *const f)
{
    int key = 0, rc = finput_node(f, &key, QUIET);
    tree_t node = NULL, tree = NULL;

    while (rc == EXIT_SUCCESS)
    {
        node = create_node(key);
        tree = insert_node(tree, node);
        rc = finput_node(f, &key, QUIET);
    }

    return tree;
}
