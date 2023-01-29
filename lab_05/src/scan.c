#include "scan.h"

int flush_input(void)
{
    int amount = 0;
    char ch;
    do
    {
        scanf("%c", &ch);
        if (ch == EOF)
            break;
        amount++;
    }
    while (ch != '\n');

    return amount;
}

int scan_integer(int *value, int min, int max)
{
    int rc;

    rc = scanf("%d", value);
    if (rc != 1)
    {
        flush_input();
        print_newline();
        fputs("Ошибка: Неверный ввод\n\n", stderr);
        return ERR_INPUT_NUMBER;
    }
    if (flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка: Требуется целое число\n\n", stderr);
        return EXIT_FAILURE;
    }

    if (*value < min || *value > max)
    {
        print_newline();
        fputs("Ошибка: Выход за границы\n\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int scan_menu(int *key)
{
    int rc;

    printf("%s", "Выбор команды: ");

    rc = scanf("%d", key);
    if (flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка: Неверный ввод\n", stderr);
        return EXIT_FAILURE;
    }

    if (rc != 1)
    {
        print_newline();
        fputs("Ошибка: Неверный ввод\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    print_newline();

    return EXIT_SUCCESS;
}
