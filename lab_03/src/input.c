#include <stdio.h>
#include <strings.h>

#include "input.h"

int input_flag(char *msg)
{
    int flag = -1;
    printf("%s", msg);

    do
    {
        fflush(stdin);
        printf("(1 - да, 0 - нет): ");
        
        scanf("%d", &flag);

        if ((flag != 1) && (flag != 0))
        {
            puts("Ошибка: Повторите ввод кода");
            flag = -1;
        }
    }
    while (flag == -1);

    return flag;
}

void input_size(char *msg, int *n, int *m)
{
    printf("%s", msg);
    fflush(stdin);

    while (scanf("%d %d", n, m) != 2)
    {
        printf("Ошибка: Некорректный ввод. Попробуйте снова: ");
        fflush(stdin);
    }
}

void input_fill(int *const fill)
{
    printf("Введите процент заполнения (0 - 100): ");
    fflush(stdin);

    while ((scanf("%d", fill) != 1) && (*fill <= 0) && (*fill > 100))
    {
        printf("Ошибка: Некорректный ввод. Попробуйте снова: ");
        fflush(stdin);
    }
}
