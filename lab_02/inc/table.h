#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "errors.h"

#define MAX_FILE_NAME 256

void add_to_table(abonent_t *table, const int size, abonent_t abonent, const int ind);
void reset_key_table(abonent_t *table, key_row_t *key_table, const int size);
void remove_from_table(abonent_t *table, int *const size, const int ind);
void swap_abonent(abonent_t *ab1, abonent_t *ab2);
void copy_abonent(abonent_t *dest_ab, abonent_t src_ab);
void init_abonent(abonent_t *abonent);

int fget_table(FILE *const f, abonent_t *table, key_row_t *key_table, const int size);
void fput_table(FILE *const f, abonent_t *table, const int size, const int code);
void fput_key_table(FILE *const f, key_row_t *key_table, const int size, const int code);
void add_to_surname_key_table(key_row_t* key_table, const int ind, key_t *new_key, const int table_ind);
int cmp_surname_key(key_t *key1, key_t *key2);
void mysort_key_table(key_row_t *key_table, const int size);
void mysort_abon_table(abonent_t *table, const int size);
void fput_table_by_key_table(FILE *const f, abonent_t *table, key_row_t *key_table, const int size, const int code);
int is_inaweek_date(struct personal_status original, struct personal_status data);

#endif // __TABLE_H__
