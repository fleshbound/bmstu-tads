#include "list.h"

typedef struct node
{
    int value;
    struct node *next;
} node_t;

struct list_queue
{
    node_t *head;
    node_t *tail;
};

list_queue_t *new_list_queue(void)
{
    list_queue_t *new_list_queue = malloc(sizeof(list_queue_t));
    if (new_list_queue == NULL)
    {
        fputs("Ошибка: Ошибка выделения памяти\n", stderr);
        return NULL;
    }

    new_list_queue->head = NULL;
    new_list_queue->tail = NULL;

    return new_list_queue;
}

void clear_list_queue(list_queue_t *list_queue)
{
    node_t *current = list_queue->head;
    node_t *prev;

    while (current != NULL)
    {
        prev = current;
        current = current->next;
        free(prev);
    }
}

void free_list_queue(list_queue_t *list_queue)
{
    clear_list_queue(list_queue);
    free(list_queue);
}

int list_queue_insert(list_queue_t *list_queue, int value, ssize_t index)
{
    size_t counter;
    node_t *prev;
    node_t *current;
    node_t *new_node;

    new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        fputs("Ошибка: Ошибка выделения памяти\n", stderr);
        return ERR_NO_MEMORY;
    }

    new_node->value = value;
    new_node->next = NULL;

    if (list_queue->head == NULL)
    {
        list_queue->head = new_node;
        list_queue->tail = new_node;
        return EXIT_SUCCESS;
    }

    if (index == -1)
    {
        list_queue->tail->next = new_node;
        list_queue->tail = new_node;
        return EXIT_SUCCESS;
    }

    counter = 0;
    prev = NULL;
    current = list_queue->head;
    while (current != NULL && counter++ < index)
    {
        prev = current;
        current = current->next;
    }

    if (prev == NULL)
    {
        new_node->next = list_queue->head;
        list_queue->head = new_node;
    }
    else
    {
        if (prev->next == NULL)
            list_queue->tail = new_node;
        new_node->next = prev->next;
        prev->next = new_node;
    }

    return EXIT_SUCCESS;
}

int list_queue_push(list_queue_t *list_queue, int value)
{
    node_t *new_node;

    new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        fputs("Ошибка: Ошибка выделения памяти\n", stderr);
        return ERR_NO_MEMORY;
    }

    new_node->value = value;
    new_node->next = NULL;

    if (list_queue->head == NULL)
    {
        list_queue->head = new_node;
        list_queue->tail = new_node;
        return EXIT_SUCCESS;
    }

    list_queue->tail->next = new_node;
    list_queue->tail = new_node;
    return EXIT_SUCCESS;
}

int list_queue_pop(list_queue_t *list_queue, int *value)
{
    node_t *current;

    current = list_queue->head;
    if (current == NULL)
        return ERR_NULL_POINTER;

    *value = current->value;
    list_queue->head = current->next;
    free(current);

    if (list_queue->head == NULL)
        list_queue->tail = NULL;

    return EXIT_SUCCESS;
}

void freed_node_add(freed_nodes_t *freed_nodes, void *address)
{
    for (size_t i = 0; i < freed_nodes->size; i++)
        if (freed_nodes->addresses[i] == address)
            return;
    freed_nodes->addresses[freed_nodes->size++] = address;
}

int list_queue_pop_address(list_queue_t *list_queue, int *value, freed_nodes_t *freed_nodes)
{
    node_t *current;

    current = list_queue->head;
    if (current == NULL)
        return ERR_NULL_POINTER;

    *value = current->value;
    list_queue->head = current->next;
    freed_node_add(freed_nodes, current);
    free(current);

    if (list_queue->head == NULL)
        list_queue->tail = NULL;

    return EXIT_SUCCESS;
}

void freed_node_delete(freed_nodes_t *freed_nodes, void *address)
{
    size_t i;
    for (i = 0; i < freed_nodes->size; i++)
        if (freed_nodes->addresses[i] == address)
        {
            freed_nodes->size--;
            break;
        }
    for (size_t j = i; j < freed_nodes->size; j++)
        freed_nodes[j] = freed_nodes[j + 1];
}

int list_queue_push_address(list_queue_t *list_queue, int value, freed_nodes_t *freed_nodes)
{
    node_t *new_node;

    new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        fputs("Ошибка: Ошибка выделения памяти\n", stderr);
        return ERR_NO_MEMORY;
    }

    new_node->value = value;
    new_node->next = NULL;
    freed_node_delete(freed_nodes, new_node);

    if (list_queue->head == NULL)
    {
        list_queue->head = new_node;
        list_queue->tail = new_node;
        return EXIT_SUCCESS;
    }

    list_queue->tail->next = new_node;
    list_queue->tail = new_node;
    return EXIT_SUCCESS;
}

int list_queue_contains(const list_queue_t *list_queue, int value)
{
    node_t *curr = list_queue->head;
    while (curr != NULL)
    {
        if (curr->value == value)
            return 1;
        curr = curr->next;
    }
    return 0;
}

size_t get_list_queue_size(const list_queue_t *list_queue)
{
    size_t size = 0;
    node_t *curr = list_queue->head;

    while (curr != NULL)
    {
        size++;
        curr = curr->next;
    }

    return size;
}

void freed_nodes_print(freed_nodes_t *freed_nodes)
{
    if (freed_nodes->size == 0)
    {
        puts("Массив адресов пуст");
        return;
    }

    puts("Массив освобожденных адресов:");
    printf("i\t| Адрес\n");
    for (size_t i = 0; i < freed_nodes->size; i++)
        printf("%lu\t| %p\n", i + 1, freed_nodes->addresses[i]);
}

void list_queue_print(list_queue_t *list_queue)
{
    node_t *curr;

    puts("ОЧЕРЕДЬ:");
    curr = list_queue->head;
    if (curr == NULL)
    {
        puts("Очередь пуста");
        return;
    }
    
    printf("(Голова очереди)\n");
    while (curr != NULL)
    {
        printf("%d | %p\n", curr->value, (void *)curr);
        curr = curr->next;
    }
}

size_t get_list_queue_size_bytes(int elements_amount)
{
    return sizeof(list_queue_t) + elements_amount * sizeof(node_t);
}
