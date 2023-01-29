#include "table.h"
#include "input.h"

void add_to_table(abonent_t *table, const int size, abonent_t abonent, const int ind)
{
    for (int i = size; i > ind; i++)
        copy_abonent(&table[i], table[i - 1]);

    copy_abonent(&table[ind], abonent);
}

void remove_from_table(abonent_t *table, int *const size, const int ind)
{
    for (int i = ind; i < *size - 1; i++)
        copy_abonent(&table[i], table[i + 1]);

    (*size)--;
}

void copy_abonent(abonent_t *dest_ab, abonent_t src_ab)
{
    memmove(dest_ab->surname, src_ab.surname, LEN_SURNAME);
    memmove(dest_ab->name, src_ab.name, LEN_NAME);
    memmove(dest_ab->address, src_ab.address, LEN_ADDRESS);
    dest_ab->status_code = src_ab.status_code;
    dest_ab->phone = src_ab.phone;

    if (src_ab.status_code == PERSONAL)
        dest_ab->status = src_ab.status;
    else if (src_ab.status_code == OFFICIAL)
    {
        memmove(dest_ab->status.official.office, src_ab.status.official.office, LEN_OFFICE);
        memmove(dest_ab->status.official.company, src_ab.status.official.company, LEN_COMPANY);
    }
}

void swap_abonent(abonent_t *ab1, abonent_t *ab2)
{
    abonent_t tmp_ab;
    init_abonent(&tmp_ab);
    copy_abonent(&tmp_ab, *ab1);
    copy_abonent(ab1, *ab2);
    copy_abonent(ab2, tmp_ab);
}

// orig by user, data from table
int is_inaweek_date(struct personal_status orig, struct personal_status data)
{
    int month_orig = 31, month_data = 31;

    if (orig.month == 2)
        month_orig -= 3;
    if ((orig.month == 4) || (orig.month == 6) ||
        (orig.month == 9) || (orig.month == 11))
        month_orig -= 1;
    
    if (data.month == 2)
        month_data -= 3;
    if ((data.month == 4) || (data.month == 6) ||
        (data.month == 9) || (data.month == 11))
        month_data -= 1;

    int days_orig = orig.day + orig.month * month_orig;
    int days_data = data.day + data.month * month_data;
    int diff = days_data - days_orig;

    if ((diff <= 7) && (diff >= 0))
        return 1;

    return 0;
}

void mysort_abon_table(abonent_t *table, const int size)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - 1 - i; j++)
            if (cmp_surname_key(table[j].surname, table[j + 1].surname) > 0)
                swap_abonent(&table[j], &table[j + 1]);
}

void init_abonent(abonent_t *abonent)
{
    memset(abonent->surname, '\0', LEN_SURNAME);
    memset(abonent->surname, '\0', LEN_SURNAME);
    memset(abonent->surname, '\0', LEN_SURNAME);
    abonent->status_code = PERSONAL;
    abonent->status.personal.year = 1;
    abonent->status.personal.day = 1;
    abonent->status.personal.month = 1;
}

// size_surname_key_table == size_table
void add_to_surname_key_table(key_row_t* key_table, const int ind, key_t *new_key, const int table_ind)
{
    key_table[ind].table_ind = table_ind;
    memmove(key_table[ind].key, new_key, LEN_SURNAME);
}

// key = surname
int cmp_surname_key(key_t *key1, key_t *key2)
{
    return strncmp(key1, key2, LEN_SURNAME);
}

void swap_key_table_row(key_row_t *row1, key_row_t *row2)
{
    int tmp_ind = 0;
    char tmp_str[LEN_SURNAME] = "";
    tmp_ind = row1->table_ind;
    row1->table_ind = row2->table_ind;
    row2->table_ind = tmp_ind;
    memmove(tmp_str, row1->key, LEN_SURNAME);
    memmove(row1->key, row2->key, LEN_SURNAME);
    memmove(row2->key, tmp_str, LEN_SURNAME);
}

void reset_key_table(abonent_t *table, key_row_t *key_table, const int size)
{
    for (int i = 0; i < size; i++)
        add_to_surname_key_table(key_table, i, table[i].surname, i);
}

// bubble sort
void mysort_key_table(key_row_t *key_table, const int size)
{
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - 1 - i; j++)
            if (cmp_surname_key(key_table[j].key, key_table[j + 1].key) > 0)
                swap_key_table_row(&key_table[j], &key_table[j + 1]);
}

void fput_key_table(FILE *const f, key_row_t *key_table, const int size, const int code)
{
    if (code == VERBOSE)
        output_key_table_header(f);

    for (int i = 0; i < size; i++)
        output_key_row(f, key_table[i], code);
}

int fget_table(FILE *const f, abonent_t *table, key_row_t *key_table, const int size)
{
    abonent_t curr_abonent;
    init_abonent(&curr_abonent);

    for (int i = 0; i < size; i++)
    {
        if (input_abonent(f, &curr_abonent, QUIET) != EXIT_SUCCESS)
        {
        //    printf("INDEX %d\n", i);
            return ERR_DATA;
        }

        add_to_table(table, i, curr_abonent, i);
        add_to_surname_key_table(key_table, i, curr_abonent.surname, i);
    }

    return EXIT_SUCCESS;
}

void fput_table(FILE *const f, abonent_t *table, const int size, const int code)
{
    if (code == VERBOSE)
        output_table_header(f);

    for (int i = 0; (strlen(table[i].surname) > 0) && (i < size); i++)
        output_abonent(f, table[i], i, code);
}

void fput_table_by_key_table(FILE *const f, abonent_t *table, key_row_t *key_table, const int size, const int code)
{
    if (code == VERBOSE)
        output_table_header(f);

    if (f == NULL) { show_err(ERR_OPEN); return; }

    for (int i = 0; i < size; i++)
        output_abonent(f, table[key_table[i].table_ind], i, code);
}

void null_table(abonent_t *table)
{
    table[0].surname[0] = '\0'; 
}
