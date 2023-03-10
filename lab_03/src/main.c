#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "input.h"
#include "alloc.h"
#include "matrices.h"
#include "output.h"
#include "time_testing.h"

int main(void)
{
    srand(time(0));

    int option;
    int flag = 0;
    int fill = 20; // percentage of non-zero elements in matrix

    int *matr1, *matr2, *matr3;
    int n, m;
    int num_of_zero_elements_1, num_of_zero_elements_2, n_z_el3;
    int num_of_zero_rows1, num_of_zero_rows2;

    int *A1, *A2, *A3;
    int *JA1, *JA2, *JA3;
    struct IA *IA1 = NULL, *IA2 = NULL, *IA3 = NULL, *tmp;


    printf("Программа предназначена для вычисления суммы матриц в обычном и CSC форматах\n");

    option = input_flag("Выполнить сложение двух матриц? ");
    if (option)
    {
        option = input_flag("Заполнить матрицы вручную? ");
        input_size("Введите размерность (кол-во рядов, кол-во столбцов): ", &n, &m);

        flag = allocate_matrices(&matr1, &matr2, &matr3, n * m);
        if (flag) 
        {
            printf("\nОшибка: Проблемы в выделением памяти");
            return flag;
        }

        // Creation of two matrices
        if (option)
        {
            printf("\nТри '-1' - признак окончания ввода\n");
            printf("\nВВОД ПЕРВОЙ МАТРИЦЫ\n");
            input_matrix(matr1, n, m);
            printf("\nВВОД ВТОРОЙ МАТРИЦЫ\n");
            input_matrix(matr2, n, m);
        } else 
        {
            input_fill(&fill);
            generate_matrix(matr1, n, m, fill);
            generate_matrix(matr2, n, m, fill);
        }

        // ПЕРВАЯ
        count_non_zero(matr1, n, m, &num_of_zero_rows1, &num_of_zero_elements_1);
        allocate_two_arrays(&A1, &JA1, num_of_zero_elements_1);

        if (num_of_zero_rows1)
        {
            IA1 = malloc(sizeof(struct IA));
            tmp = IA1;

            for (int i = 0; i < num_of_zero_rows1 - 1; i++)
            {
                tmp->next = malloc(sizeof(struct IA));
                tmp = tmp->next;
            }

            tmp->next = NULL;
        }

        convert_matrix(matr1, n, m, A1, JA1, IA1);

        // ВТОРАЯ
        count_non_zero(matr2, n, m, &num_of_zero_rows2, &num_of_zero_elements_2);
        allocate_two_arrays(&A2, &JA2, num_of_zero_elements_2);

        if (num_of_zero_rows2) {
            IA2 = malloc(sizeof(struct IA));
            tmp = IA2;

            for (int i = 0; i < num_of_zero_rows2 - 1; i++) 
            {
                tmp->next = malloc(sizeof(struct IA));
                tmp = tmp->next;
            }

            tmp->next = NULL;
        }

        convert_matrix(matr2, n, m, A2, JA2, IA2);

        // СЛОЖЕНИЕ
        allocate_two_arrays(&A3, &JA3, num_of_zero_elements_1 + num_of_zero_elements_2);
        IA3 = malloc(sizeof(struct IA));
        IA3->Nk = -1;
        IA3->i = -1;
        IA3->next = NULL;

        add_matrices(matr1, matr2, matr3, n, m);
        add_matrices_scr(A1, JA1, IA1, num_of_zero_elements_1, A2, JA2, IA2, num_of_zero_elements_2, A3, JA3, IA3, &n_z_el3);

        option = input_flag("\nВывести матрицы на экран? ");
        if (option) {
            puts("\nПЕРВАЯ МАТРИЦА");
            print_matrix(matr1, n, m);
            print_scr(A1, JA1, IA1, num_of_zero_elements_1);

            puts("\nВТОРАЯ МАТРИЦА");
            print_matrix(matr2, n, m);
            print_scr(A2, JA2, IA2, num_of_zero_elements_2);

            puts("\nРЕЗУЛЬТАТ СЛОЖЕНИЯ");
            print_matrix(matr3, n, m);
            print_scr(A3, JA3, IA3, n_z_el3);
        }

        // ОСВОБОЖДЕНИЕ
        free_all(matr1, A1, JA1, IA1);
        free_all(matr2, A2, JA2, IA2);
        free_all(matr3, A3, JA3, IA3);
    }

    // ОЦЕНКА ЭФФЕКТИВНОСТИ
    fflush(stdin);
    option = input_flag("\nПровести оценку эффективности? ");

    if (option)
    {
        puts("АНАЛИЗ ЭФФЕКТИВНОСТИ");
        puts(" Элементов | g(%) | Не CSR:   Время   Размер (Б) | CSR:       Время  Размер (Б) ");
        time_testing(10, 10, 5);
        time_testing(10, 10, 20);
        time_testing(10, 10, 50);
        time_testing(100, 100, 5);
        time_testing(100, 100, 20);
        time_testing(100, 100, 50);
        time_testing(1000, 1000, 5);
        time_testing(1000, 1000, 20);
        time_testing(1000, 1000, 50);
        time_testing(1000, 1000, 90);
    }

    //option = input_flag("");
    return 0;
}
