#include <stdlib.h>

#include "bstree.h"

struct tree_node
{
    key_t key;
    tree_t left;
    tree_t right;
};

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

// Добавление элемента в дерево
tree_t insert_node(tree_t tree, tree_t node)
{
    if (tree == NULL)
        return node;

    if (node->key < tree->key)
        tree->left = insert_node(tree->left, node);
    else if (node->key > tree->key)
        tree->right = insert_node(tree->right, node);
    
    return tree;
}

tree_t get_min_key_node(tree_t tree)
{
    if (tree->left == NULL) return tree;

    return get_min_key_node(tree->left);
}

tree_t delete_node(tree_t tree, const key_t key)
{
    if (tree->key == key)
    {
        if ((tree->left == NULL) && (tree->right == NULL))
            return NULL;
    
        if (tree->left == NULL)
        {
            tree_t tmp = tree->right;
            free(tree);
            return tmp;
        }

        if (tree->right == NULL)
        {
            tree_t tmp = tree->left;
            free(tree);
            return tmp;
        }

        tree_t min_right = get_min_key_node(tree->right);
        tree->key = min_right->key;
        tree->right = delete_node(tree->right, min_right->key);
        
        return tree;
    }

    if (key < tree->key)
    {
        if (tree->left == NULL)
        {
            printf("Примечание: узел не найден\n");
            return tree;
        }

        tree->left = delete_node(tree->left, key);

        return tree;
    }

    if (tree->right == NULL)
    {
        printf("Примечание: узел не найден\n");
        return tree;
    }

    tree->right = delete_node(tree->right, key);

    return tree;
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

tree_t search_node(tree_t tree, const key_t key)
{
    if (tree == NULL)
        return NULL;

    if (tree->key == key)
        return tree;

    tree_t tmp1 = search_node(tree->left, key);
    
    if (tmp1 != NULL) 
        return tmp1;

    tree_t tmp2 = search_node(tree->right, key);

    return tmp2;
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
        fprintf(f, "%d -> %d [label=\"L\"];\n", node->key, node->left->key);

    if (node->right)
        fprintf(f, "%d -> %d [label=\"R\"];\n", node->key, node->right->key);
}

void store_sorted(tree_t tree, int *data, int *const i)
{
    if (tree == NULL)
        return;
    
    store_sorted(tree->left, data, i);
    data[(*i)++] = tree->key;
    store_sorted(tree->right, data, i);
}

void print_node(tree_t node, void *param)
{
    FILE *f = param;

    fprintf(f, "%d: %d, %d\n", node->key,
           (node->left != NULL) ? node->left->key : node->key,
           (node->right != NULL) ? node->right->key : node->key);
}

size_t get_nodes_count(tree_t tree, int level)
{
    if (tree == NULL) return 0;
    if (0 == level) return 1;
    return get_nodes_count(tree->left, level - 1)
           + get_nodes_count(tree->right, level - 1);
}

void get_all_count(tree_t tree, size_t *const count)
{
    if (tree == NULL)
        return;

    (*count)++;
    get_all_count(tree->left, count);
    get_all_count(tree->right, count);
}

int get_height(tree_t node)
{
    if (node == NULL)
        return -1;

    int left_height = get_height(node->left);
    int right_height = get_height(node->right);
    
    if (left_height > right_height)
        return left_height + 1;
    else
        return right_height + 1;
}

void print_node_lookup(tree_t node, void *param)
{
    FILE *f = param;

    fprintf(f, "%d ", node->key);
}
