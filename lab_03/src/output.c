#include <stdio.h>
#include "output.h"
#include "matrices.h"

void print_matrix(const int *matr, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", matr[i * m + j]);

        printf("\n");
    }
}

void print_array(const int *arr, int len)
{
    for (int i = 0; i < len; i++)
        printf("%d ", arr[i]);
}

void print_list(struct IA *IA)
{
    struct IA *tmp = IA;

    printf("\n  Non-zero indexes: ");

    if (tmp != NULL)
        while (tmp != NULL)
        {
            printf("%d ", tmp->i);
            tmp = tmp->next;
        }


    tmp = IA;
    printf("\n  ANk: ");

    if (tmp != NULL)
        while (tmp != NULL)
        {
            printf("%d ", tmp->Nk);
            tmp = tmp->next;
        }
}

void print_scr(const int *A, const int *JA, struct IA *IA, const int n_z_el)
{
    printf("\nA: ");
    print_array(A, n_z_el);

    printf("\nJA: ");
    print_array(JA, n_z_el);

    printf("\nIA:");
    print_list(IA);

    puts(" ");
}
