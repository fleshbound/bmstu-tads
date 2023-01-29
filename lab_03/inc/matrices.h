#ifndef MATRICES_H
#define MATRICES_H

#define ELEM_READ_COUNT 3

struct IA
{
    int i;
    int Nk;
    struct IA *next;
};

void generate_matrix(int *matr, int n, int m, int fill);
void input_matrix(int *matr, int n, int m);

void count_non_zero(const int *matr, int n, int m, int *non_zero_rows, int *non_zero_elements);
void convert_matrix(const int *matr, int n, int m, int *A, int *JA, struct IA *IA);

void add_matrices(const int *matr1, const int *matr2, int *matr3, int n, int m);
void add_matrices_scr(const int *A1, const int *JA1, struct IA *IA1, int num_of_zero_elements_1,
                           const int *A2, const int *JA2, struct IA *IA2, int num_of_zero_elements_2,
                           int *A3, int *JA3, struct IA *IA3, int *num_of_zero_elements_3);

void copy_row(const int *A1, const int *JA1, struct IA *IA1, int max_iel,
              int *A3, int *JA3, struct IA *IA3, int *num_of_zero_elements_3);
void add_rows(const int *A1, const int *JA1, struct IA *IA1, int max_curr_1_index,
              const int *A2, const int *JA2, struct IA *IA2, int max_curr_2_index,
              int *A3, int *JA3, struct IA *IA3, int *num_of_zero_elements_3);

#endif // MATRICES_H
