#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "output.h"
#include "matrices.h"

void generate_matrix(int *matr, int n, int m, int fill)
{
    int chance;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
        {
            chance = rand() % 100;

            if (chance > fill)
                matr[i * m + j] = 0;
            else
                matr[i * m + j] = rand() % 10;
        }
}

void input_matrix(int *matr, int n, int m)
{
    int i = 0, j = 0, value = 0;// flag = 0;

    fflush(stdin);
    do
    {
        
        /* if (flag) */
        /*     fgetc(stdin); */

        if (i >= 0 && j >= 0 && i < n && j < m)
            matr[i * m + j] = value;
        else
            break;

        printf("Введите элемент (i, j, значение): ");
        /* flag = 1; */
        fflush(stdin);
    }
    while (scanf("%d %d %d", &i, &j, &value) == ELEM_READ_COUNT);
}

void count_non_zero(const int *matr, int n, int m, int *non_zero_rows, int *non_zero_elements)
{
    *non_zero_rows = 0;
    *non_zero_elements = 0;

    int row_counted = 0;

    for (int i = 0; i < n; i++)
    {
        row_counted = 0;

        for (int j = 0; j < m; j++)
            if (matr[i * m + j] != 0)
            {
                (*non_zero_elements)++;

                if (!row_counted)
                {
                    row_counted = 1;
                    (*non_zero_rows)++;
                }
            }
    }
}

// Перевод матрицы из обычного вида в вид CSC
void convert_matrix(const int *matr, int n, int m, int *A, int *JA, struct IA *IA)
{
    int A_curr = 0; // Текущий индекс в массиве элементов
    int first_in_row = 1;

    for (int i = 0; i < n; i++)
    {
        first_in_row = 1;

        for (int j = 0; j < m; j++)
            if (matr[i * m + j] != 0) // Если элемент ненулевой
            {
                A[A_curr] = matr[i * m + j]; // Текущий элемент массива элементов
                JA[A_curr] = j; // индекс столбца для текущего элемента

                if (first_in_row) // если он - первый в строке, заполняем соотв.
                                  // элемент IA
                {
                    first_in_row = 0; 
                    IA->Nk = A_curr; // номер JA и A с которого начинается 
                    IA->i = i; // номер строки
                    IA = IA->next;
                }

                A_curr++;
            }
    }
}

void add_matrices(const int *matr1, const int *matr2, int *matr3, int n, int m)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matr3[i * m + j] = matr1[i * m + j] + matr2[i * m + j];
}

void add_matrices_scr(const int *A1, const int *JA1, struct IA *IA1, int num_of_non_zero_elements_1,
                           const int *A2, const int *JA2, struct IA *IA2, int num_of_non_zero_elements_2,
                           int *A3, int *JA3, struct IA *IA3, int *num_of_non_zero_elements_3)
{
    *num_of_non_zero_elements_3 = 0;

    struct IA *tmp1 = IA1, *tmp2 = IA2, *tmp3 = IA3;

    while (tmp1 && tmp2) // Пока складываемые списки не пусты 
                         // (в обоих есть ненулевые строки)
    {
        if (tmp1->i < tmp2->i) // Если текущая ненулевая строка 1-й матрицы имеет 
                               // меньший индекс, чем текущая ненулевая строка 2-й матрицы
        {
            // Просто копируем ее в результирующую матрицу
            copy_row(A1, JA1, tmp1, num_of_non_zero_elements_1, A3, JA3, tmp3, num_of_non_zero_elements_3);
            tmp1 = tmp1->next;
        }
        else if (tmp2->i < tmp1->i) // Если наоборот
        {
            // Копируем из 2-й в результирующую
            copy_row(A2, JA2, tmp2, num_of_non_zero_elements_2, A3, JA3, tmp3, num_of_non_zero_elements_3);
            tmp2 = tmp2->next;
        }
        else // Если индексы строк совпали
        {
            // Складываем строки
            add_rows(A1, JA1, tmp1, num_of_non_zero_elements_1, A2, JA2, tmp2, num_of_non_zero_elements_2, A3, JA3, tmp3, num_of_non_zero_elements_3);
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }

        if (tmp3->next)
            tmp3 = tmp3->next;
    }

    while (tmp1) // Если еще остались ненулевые строки в 1-й матрице
    {
        // Копируем их в 3-ю
        copy_row(A1, JA1, tmp1, num_of_non_zero_elements_1, A3, JA3, tmp3, num_of_non_zero_elements_3);
        tmp1 = tmp1->next;

        if (tmp3->next)
            tmp3 = tmp3->next;
    }

    while (tmp2) // Аналогично со 2-й
    {
        copy_row(A2, JA2, tmp2, num_of_non_zero_elements_2, A3, JA3, tmp3, num_of_non_zero_elements_3);
        tmp2 = tmp2->next;

        if (tmp3->next)
            tmp3 = tmp3->next;
    }
}

// Выполянет копирование ряда из 1-й матрицы в 3-ю
void copy_row(const int *A1, const int *JA1, struct IA *IA1, int max_iel,
              int *A3, int *JA3, struct IA *IA3, int *num_of_non_zero_elements_3)
{
    // Определяем пределы копирования в рамках массива A
    int i_start = IA1->Nk;
    int i_end;

    if (IA1->next)
    {
        i_end = (IA1->next)->Nk;
    }
    else
    {
        i_end = max_iel;
    }

    // Создание нового ряда, если 3-я матрица не пуста
    if (IA3->i != -1)
    {
        IA3->next = malloc(sizeof(struct IA));
        IA3 = IA3->next;
        IA3->next = NULL;
    }

    // Запись данных из 1-го в 3-й
    IA3->i = IA1->i;
    IA3->Nk = *num_of_non_zero_elements_3;

    for (int i = i_start; i < i_end; i++)
    {
        A3[*num_of_non_zero_elements_3] = A1[i];
        JA3[*num_of_non_zero_elements_3] = JA1[i];
        (*num_of_non_zero_elements_3)++;
    }
}

// Выполняет сложение ненулевых рядов 
void add_rows(const int *A1, const int *JA1, struct IA *IA1, int max_curr_1_index,
              const int *A2, const int *JA2, struct IA *IA2, int max_curr_2_index,
              int *A3, int *JA3, struct IA *IA3, int *num_of_non_zero_elements_3)
{
    // Создание нового ряда, если 3-я матрица не пуста
    if (IA3->i != -1)
    {
        IA3->next = malloc(sizeof(struct IA));
        IA3 = IA3->next;
        IA3->next = NULL;
    }

    IA3->i = IA1->i;
    IA3->Nk = *num_of_non_zero_elements_3;

    // Поиск границ сложения (индексов)
    int row_start_ind_1 = IA1->Nk; // стратовый элемент для сложения 1-й строки
    int row_end_ind_1, curr_1_index = row_start_ind_1;

    // Если в первой матрице еще есть ненулевая строка, финишный индекс - ее начала
    if (IA1->next)
        row_end_ind_1 = (IA1->next)->Nk;
    else
        row_end_ind_1 = max_curr_1_index;

    // Аналогично со 2-й
    int row_start_ind_2 = IA2->Nk;
    int row_end_ind_2, curr_2_index = row_start_ind_2;

    if (IA2->next)
        row_end_ind_2 = (IA2->next)->Nk;
    else
        row_end_ind_2 = max_curr_2_index;

    // Сложение строк
    while ((curr_1_index < row_end_ind_1) && (curr_2_index < row_end_ind_2))
    {
        if (JA1[curr_1_index] < JA2[curr_2_index])
        {
            JA3[*num_of_non_zero_elements_3] = JA1[curr_1_index];
            A3[*num_of_non_zero_elements_3] = A1[curr_1_index++];
        }
        else if (JA2[curr_2_index] < JA1[curr_1_index])
        {
            JA3[*num_of_non_zero_elements_3] = JA2[curr_2_index];
            A3[*num_of_non_zero_elements_3] = A2[curr_2_index++];
        }
        else
        {
            JA3[*num_of_non_zero_elements_3] = JA1[curr_1_index];
            A3[*num_of_non_zero_elements_3] = A1[curr_1_index++] + A2[curr_2_index++];
        }
        (*num_of_non_zero_elements_3)++;
    }

    // Если какие-то элементы остались в 1-й или 2-й матрице
    while (curr_1_index < row_end_ind_1)
    {
        JA3[*num_of_non_zero_elements_3] = JA1[curr_1_index];
        A3[*num_of_non_zero_elements_3] = A1[curr_1_index++];
        (*num_of_non_zero_elements_3)++;
    }

    while (curr_2_index < row_end_ind_2)
    {
        JA3[*num_of_non_zero_elements_3] = JA2[curr_2_index];
        A3[*num_of_non_zero_elements_3] = A2[curr_2_index++];
        (*num_of_non_zero_elements_3)++;
    }
}
