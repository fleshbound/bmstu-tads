#include <stdlib.h>

#include "avltree.h"
#include "errors.h"

struct avltree_node
{
    key_t key;
    avltree_t left;
    avltree_t right;
    size_t height;
};

static size_t comparisons;
static size_t rc;

size_t avl_get_size(void)
{
    return sizeof(struct avltree_node);
}

int get_height(avltree_t node)
{
    if (node == NULL)
        return 0;

    return node->height;
}

int _balance_factor(avltree_t node)
{
    if (node == NULL)
        return 0;

    /* printf("\nBAL %d -- h_r: %d, h_l: %d", node->key, get_height(node->right), get_height(node->left)); */
    return get_height(node->right) - get_height(node->left);
}

size_t hmax(const size_t h1, const size_t h2)
{
    return (h1 >= h2) ? h1 : h2;
}

avltree_t avl_create_node(const key_t key)
{
    avltree_t node = malloc(sizeof(struct avltree_node));

    if (node != NULL)
    {
        node->key = key;
        node->left = NULL;
        node->right = NULL;
        node->height = 0;
    }

    return node;
}

void _avl_delete_tree(avltree_t tree)
{
    if (tree == NULL)
        return;

    _avl_delete_tree(tree->left);
    _avl_delete_tree(tree->right);

    free(tree);
}

void avl_delete_tree(avltree_t *tree)
{
    _avl_delete_tree(*tree);
    *tree = NULL;
}

avltree_t rotate_left(avltree_t node)
{
    /* printf("\nrotate left!"); */
    avltree_t r = node->right;
    node->right = r->left;
    r->left = node;

    node->height = hmax(get_height(node->left), get_height(node->right)) + 1;
    r->height = hmax(get_height(r->left), get_height(r->right)) + 1;

    return r;
}

avltree_t rotate_right(avltree_t node)
{
    /* printf("\nrotate right!"); */
    avltree_t l = node->left;
    node->left = l->right;
    l->right = node;

    node->height = hmax(get_height(node->left), get_height(node->right)) + 1;
    l->height = hmax(get_height(l->left), get_height(l->right)) + 1;

    return l;
}

avltree_t rotate_right_left(avltree_t node)
{
    /* printf("\nrotate right-left!"); */
    node->right = rotate_right(node->right);
    node = rotate_left(node);
    node->height = hmax(get_height(node->left), get_height(node->right)) + 1;
    
    return node;
}

avltree_t rotate_left_right(avltree_t node)
{
    /* printf("\nrotate left-right!"); */
    node->left = rotate_left(node->left);
    node = rotate_right(node);
    node->height = hmax(get_height(node->left), get_height(node->right)) + 1;

    return node;
}

avltree_t _rebalance(avltree_t tree)
{
    /* printf("IM REBALANCE!!!\n"); */
    int bal = _balance_factor(tree),
        lbal = _balance_factor(tree->left),
        rbal = _balance_factor(tree->right);
    
    /* printf("\ntree: %d, bal: %d, left bal: %d, right bal: %d\n", */
           /* tree->key, bal, lbal, rbal); */

    if ((bal < -1) && (lbal == -1))
        return rotate_right(tree);

    if ((bal > 1) && (rbal == 1))
        return rotate_left(tree);

    if ((bal < -1) && (lbal == 1))
        return rotate_left_right(tree);

    if ((bal > 1) && (rbal == -1))
        return rotate_right_left(tree);

    return tree;
}

avltree_t _insert_node(avltree_t tree, avltree_t node)
{
    /* printf("\nInsert: %d, current node: %d", node->key, (tree != NULL) ? tree->key : -228228); */

    if (tree == NULL)
    {
        node->height = 1;
        return node;
    }

    if (node->key == tree->key)
    {
        rc = ERR_EXISTING_KEY;
        return tree;
    }

    if (node->key < tree->key)
        tree->left = _insert_node(tree->left, node);
    else
        tree->right = _insert_node(tree->right, node);

    tree->height = 1 + hmax(get_height(tree->left), get_height(tree->right));

    return _rebalance(tree);
}

avltree_t _search_node(avltree_t tree, const key_t key)
{
    comparisons++;

    if (tree == NULL)
        return NULL;

    if (key == tree->key)
        return tree;

    if (key < tree->key)
        return _search_node(tree->left, key);

    return _search_node(tree->right, key);
}

void rebalance(avltree_t *tree)
{
    if (*tree == NULL)
        return;

    *tree = _rebalance(*tree);
}

size_t avl_get_comparisons(void)
{
    return comparisons;
}

avltree_t avl_insert_node(avltree_t tree, avltree_t node)
{
    rc = EXIT_SUCCESS;
    avltree_t new_tree = _insert_node(tree, node);

    if ((_balance_factor(new_tree) < -1) || (_balance_factor(new_tree) > 1))
        rebalance(&new_tree);

    return new_tree;
}

void avl_reset_rc(void)
{
    rc = EXIT_SUCCESS;
}

int avl_get_rc(void)
{
    return rc;
}

avltree_t avl_search_node(avltree_t tree, const key_t key)
{
    comparisons = 0;
    return _search_node(tree, key);
}

void avl_apply_tree(const int code, avltree_t tree, void (*f)(avltree_t, void*), void *arg)
{
    if (tree == NULL)
        return;

    if (code == PRE) f(tree, arg);
    
    avl_apply_tree(code, tree->left, f, arg);
    
    if (code == IN) f(tree, arg);
    
    avl_apply_tree(code, tree->right, f, arg);
    
    if (code == POST) f(tree, arg);
}

void avl_node_to_dot(avltree_t node, void *param)
{
    FILE *f = param;

    if ((node->left == NULL) && (node->right == NULL))
        fprintf(f, "%d;\n", node->key);

    fprintf(f, "%d [label=\"%d\"];\n", node->key, node->key);

    if (node->left)
        fprintf(f, "%d -> %d [label=\"L\"];\n",
                node->key, node->left->key);

    if (node->right)
        fprintf(f, "%d -> %d [label=\"R\"];\n",
                node->key, node->right->key);
}

void avl_print_node(avltree_t node)
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

    printf(", H = %d]\n", get_height(node));
}
