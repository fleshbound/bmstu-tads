#include <stdlib.h>

#include "bstree.h"
#include "errors.h"

struct tree_node
{
    key_t key;
    tree_t left;
    tree_t right;
};

static size_t comparisons;
static int rc;

size_t bst_get_size(void)
{
    return sizeof(struct tree_node);
}

// Создание нового элемента дерева
tree_t create_node(const key_t key)
{
    tree_t node = malloc(sizeof(struct tree_node));

    if (node != NULL)
    {
        node->key = key;
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

int get_rc(void)
{
    return rc;
}

void reset_rc(void)
{
    rc = EXIT_SUCCESS;
}

// Добавление элемента в дерево
tree_t insert_node(tree_t tree, tree_t node)
{
    rc = EXIT_SUCCESS;

    if (tree == NULL)
        return node;

    if (node->key < tree->key)
        tree->left = insert_node(tree->left, node);
    else if (node->key > tree->key)
        tree->right = insert_node(tree->right, node);
    else if (node->key == tree->key)
        rc = ERR_EXISTING_KEY;

    return tree;
}

tree_t get_min_key_node(tree_t tree)
{
    if (tree->left == NULL) return tree;

    return get_min_key_node(tree->left);
}

void _delete_tree(tree_t tree)
{
    if (tree == NULL)
        return;

    _delete_tree(tree->left);
    _delete_tree(tree->right);

    free(tree);
}

void delete_tree(tree_t *tree)
{
    _delete_tree(*tree);
    *tree = NULL;
}

tree_t _bssearch_node(tree_t tree, const key_t key)
{
    if (tree == NULL)
        return NULL;
    
    comparisons++;

    if (tree->key == key)
        return tree;
    
    tree_t tmp1 = _bssearch_node(tree->left, key);
    
    if (tmp1 != NULL) 
        return tmp1;
    
    tree_t tmp2 = _bssearch_node(tree->right, key);

    return tmp2;
}

tree_t search_node(tree_t tree, const key_t key)
{
    return _bssearch_node(tree, key);
}

void null_comparisons(void)
{
    comparisons = 0;
}

size_t get_comparisons(void)
{
    return comparisons;
}

void apply_tree(const int code, tree_t tree, void (*f)(tree_t, void*), void *arg)
{
    if (tree == NULL)
        return;

    if (code == PRE) f(tree, arg);
    
    apply_tree(code, tree->left, f, arg);
    
    if (code == IN) f(tree, arg);
    
    apply_tree(code, tree->right, f, arg);
    
    if (code == POST) f(tree, arg);
}

void node_to_dot(tree_t node, void *param)
{
    FILE *f = param;

    if ((node->left == NULL) && (node->right == NULL))
        fprintf(f, "%d;\n", node->key);

    if (node->left)
        fprintf(f, "%d -> %d [label=\"L\"];\n",
                node->key, node->left->key);

    if (node->right)
        fprintf(f, "%d -> %d [label=\"R\"];\n",
                node->key, node->right->key);
}

void print_node(tree_t node)
{
    printf("%d [ L: ", node->key);

    if (node->left != NULL)
        printf("%d", node->left->key);
    else
        printf("-");

    printf(", R: ");

    if (node->right != NULL)
        printf("%d", node->right->key);
    else
        printf("-");

    printf("]\n");
}
