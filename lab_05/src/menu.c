#include "menu.h"
#include "print.h"

void print_menu(void)
{
    puts("ДОСТУПНЫЕ КОМАНДЫ");
    puts("1. Добавить элемент (МАССИВ)");
    puts("2. Удалить элемент (МАССИВ)");
    puts("3. Показать содержимое очереди (МАССИВ)");
    puts("4. Добавить элемент (СПИСОК)");
    puts("5. Удалить элемент (СПИСОК)");
    puts("6. Показать содержимое очереди (СПИСОК)");
    puts("7. Оценка фрагментации");
    puts("8. Моделирование работы аппарата (СПИСОК)");
    puts("9. Сравнить эффективность работы очередей");
    puts("0. Выход");
}

int menu_loop(void)
{
    int rc;
    int key = -1;
    int value;
    list_queue_t *list_queue;
    array_queue_t *array_queue;
    freed_nodes_t freed_nodes = { .size = 0 };

    list_queue = new_list_queue();

    if (list_queue == NULL)
        return ERR_NO_MEMORY;

    array_queue = new_array_queue(INITIAL_SIZE);

    if (array_queue == NULL)
    {
        free_list_queue(list_queue);
        return ERR_NO_MEMORY;
    }

    do
    {
        print_menu();

        rc = scan_menu(&key);
        if (rc != EXIT_SUCCESS)
            continue;

        switch (key)
        {
            case 0:
                break;
            case 1:
                printf("Введите значение элемента [%d;%d]: ", MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                rc = scan_integer(&value, MIN_DEMO_VALUE, MAX_DEMO_VALUE);

                if (rc != EXIT_SUCCESS)
                    break;

                rc = array_queue_push(array_queue, value);

                if (rc != EXIT_SUCCESS)
                    break;

                puts("Элемент был успешно вставлен");
                break;
            case 2:
                rc = array_queue_pop(array_queue, &value);

                if (rc != EXIT_SUCCESS)
                    break;

                printf("Элемент %d был успешно удален\n", value);
                break;
            case 3:
                array_queue_print(array_queue);
                break;
            case 4:
                printf("Введите значение элемента [%d;%d]: ", MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                rc = scan_integer(&value, MIN_DEMO_VALUE, MAX_DEMO_VALUE);

                if (rc != EXIT_SUCCESS)
                    break;

                rc = list_queue_push_address(list_queue, value, &freed_nodes);

                if (rc != EXIT_SUCCESS)
                    break;

                puts("Элемент был успешно вставлен");
                break;
            case 5:
                rc = list_queue_pop_address(list_queue, &value, &freed_nodes);

                if (rc != EXIT_SUCCESS)
                    break;

                printf("Элемент %d был успешно удален\n", value);
                break;
            case 6:
                list_queue_print(list_queue);
                break;
            case 7:
                freed_nodes_print(&freed_nodes);
                break;
            case 8:
                simulate_list_queue();
                break;
            case 9:
                compare_efficiency();
                break;
            default:
                fprintf(stderr, "Ошибка: Неверный формат\n");
        }
        print_newline();
    }
    while (key);

    free_list_queue(list_queue);
    free_array_queue(array_queue);

    return EXIT_SUCCESS;
}
