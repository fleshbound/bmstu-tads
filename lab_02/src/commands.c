#include "commands.h"

#ifndef NMAX
#define NMAX 3000
#endif

#define LEN_COM 255
#define ITERATIONS 200

abonent_t abon_table[NMAX];
key_row_t key_table[NMAX];
int curr_size = 0;

int input_command(void)
{
    fprintf(stdout, MSG_COM_GET);
    char tmp_s[LEN_COM] = "";

    if (fgets(tmp_s, LEN_COM, stdin) == NULL)
        return ERR_COM_IND;

    clear_str(tmp_s);
    if (strlen(tmp_s) == 0)
    {
        show_err(ERR_EMPTYSTR);
        return ERR_COM_IND;
    }

    char *ptr;
    int tmp = strtol(tmp_s, &ptr, 10);

    if ((tmp == 0) && (strlen(ptr) != 0))
    {
        show_err(ERR_DATA);
        return ERR_COM_IND;        
    }

    if ((tmp < MIN_COM_IND) || (tmp > MAX_COM_IND))
    {
        show_err(ERR_DATA);
        tmp = ERR_COM_IND;
    }

    return tmp;
}

void command_file_fill_tables(void)
{
    char filename[MAX_FILE_NAME + 1] = "";

    if (input_filename(filename)) { show_err(ERR_DATA); return; }

    if (strlen(filename) == 0) { show_err(ERR_EMPTYSTR); return; }
   
    FILE *f = fopen(filename, "rt");
    
    if (f == NULL) { show_err(ERR_OPEN); return; }

    int size = 0;
    if (input_size(&size)) { fprintf(stderr, "size:\n"); show_err(ERR_DATA); return; }

    int err_code = EXIT_SUCCESS;
    err_code = fget_table(f, abon_table, key_table, size);

    if (fclose(f) == EOF) { show_err(ERR_CLOSE); curr_size = 0; return; }

    if (err_code) { show_err(err_code); curr_size = 0; }
    else 
    {
        printf("Таблицы успешно заполнены\n");
        curr_size = size;
    }
}

void command_show_abon_table(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    fput_table(stdout, abon_table, curr_size, VERBOSE); 
}

void command_show_key_table(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    fput_key_table(stdout, key_table, curr_size, VERBOSE);
}

void command_add_abon_by_user(void)
{
    if (curr_size == NMAX) { show_err(ERR_FULL_TABLE); return; }

    abonent_t new_abonent;
    init_abonent(&new_abonent);
    reset_key_table(abon_table, key_table, curr_size);

    int rc = input_abonent(stdin, &new_abonent, VERBOSE);
    if (rc) { show_err(rc); return; }

    add_to_table(abon_table, curr_size, new_abonent, curr_size);
    add_to_surname_key_table(key_table, curr_size, new_abonent.surname, curr_size);
    curr_size++;
    printf("Новый абонент был успешно добавлен в конец таблицы\n");
}

void command_del_abon_by_value(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    char value[LEN_SURNAME] = "";

    if (input_surname(stdin, value)) { show_err(ERR_DATA); return; }

    int i = 0, q = 0;
    abonent_t *ptr_curr = abon_table, *ptr_end = abon_table + curr_size;
    
    while (ptr_curr < ptr_end)
    {
        if (cmp_surname_key(value, ptr_curr->surname) == 0)
        {
            remove_from_table(abon_table, &curr_size, i);
            q++;
            ptr_end = abon_table + curr_size;
        }
        else
        {
            ptr_curr++;
            i++;
        }
    }

    reset_key_table(abon_table, key_table, curr_size);
    printf("Записи с фамилией %s были успешно удалены (%d)\n", value, q);
}

void command_sort_key_table(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }
    
    mysort_key_table(key_table, curr_size);
    fput_key_table(stdout, key_table, curr_size, VERBOSE);
    printf("Таблица ключей была успешно отсортирована\n");
}

void command_sort_abon_table(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    mysort_abon_table(abon_table, curr_size);
    fput_table(stdout, abon_table, curr_size, VERBOSE);
    printf("Таблица абонентов была успешно отсортирована\n");
}

void command_keysort_abon_table(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }
    
    mysort_key_table(key_table, curr_size);
    fput_table_by_key_table(stdout, abon_table, key_table, curr_size, VERBOSE);
    printf("Таблица абонентов была успешно отсортирована с помощью таблицы ключей\n");
}

void command_clear_tables(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    for (int i = 0; i < curr_size; i++)
    {
        clear_abonent(&abon_table[i]);
        clear_str(key_table[i].key);
    }

    curr_size = 0;
    printf("Таблица была успешно очищена\n");
}

void command_show_rules(void)
{
    fprintf(stdout, MSG_RULES);
}

void command_show_menu(void)
{
    fprintf(stdout, MSG_COM1"\n");
    fprintf(stdout, MSG_COM2"\n");
    fprintf(stdout, MSG_COM3"\n");
    fprintf(stdout, MSG_COM4"\n");
    fprintf(stdout, MSG_COM5"\n");
    fprintf(stdout, MSG_COM6"\n");
    fprintf(stdout, MSG_COM7"\n");
    fprintf(stdout, MSG_COM8"\n");
    fprintf(stdout, MSG_COM9"\n");
    fprintf(stdout, MSG_COM10"\n");
    fprintf(stdout, MSG_COM11"\n");
    fprintf(stdout, MSG_COM12"\n");
    fprintf(stdout, MSG_COM13"\n");
    fprintf(stdout, MSG_COM0"\n");
}

int cmp_abon(const void *a, const void *b)
{
    return cmp_surname_key(((abonent_t *) a)->surname, ((abonent_t *) b)->surname);
}

int cmp_key(const void *a, const void *b)
{
    return strncmp(((key_row_t *) a)->key, ((key_row_t *) b)->key, LEN_SURNAME);
}

void command_show_diff_sorts(void)
{
    curr_size = NMAX;
    
    char filename[LEN_FILENAME] = "";

    while (input_filename(filename)) {}
    
    FILE *in = fopen(filename, "rt");
    if (in == NULL) { show_err(ERR_OPEN); return; }
   
    int size = 0;
    if (input_size(&size)) { show_err(ERR_DATA); return; }
    curr_size = size;

    fget_table(in, abon_table, key_table, curr_size);
    if (fclose(in) == EOF) { show_err(ERR_CLOSE); return; }

    printf("\nЕдиницы измерения: us\n");
    printf("Кол-во записей: %d\n", curr_size);
    printf("Кол-во итераций: %d", ITERATIONS);
   
    clock_t curr_time = 0;
    /* Сортировка: mysort (bubble), таблица: ключей */
    /***/
    clock_t sum_time4 = 0;
    clock_t sum_key_time = 0, curr_key_time = 0;

    for (int i = 0; i < ITERATIONS; i++)
    {
        reset_key_table(abon_table, key_table, curr_size);
        curr_time = clock();
        mysort_key_table(key_table, curr_size);
        curr_time = clock() - curr_time;
        sum_time4 += curr_time;
    
        curr_key_time = clock();

        for (int j = 0; j < curr_size; j++)
            if (&abon_table[key_table[j].table_ind]) 
                curr_time = 0;

        curr_key_time = clock() - curr_key_time;
        sum_key_time += curr_key_time;
    }

    sum_key_time = sum_key_time / ITERATIONS;
    sum_time4 = sum_time4 / ITERATIONS;
    long long unsigned time_mysort_key = sum_time4 / (CLOCKS_PER_SEC / 1000000LLU);
    long long unsigned time_myaccess_key = sum_key_time / (CLOCKS_PER_SEC / 1000000LLU);
    /***/
    
    /* Сортировка: qsort, таблица: ключей */
    /***/
    clock_t sum_time2 = 0;
    sum_key_time = 0;
    
    for (int i = 0; i < ITERATIONS; i++)
    {
        reset_key_table(abon_table, key_table, curr_size);
        curr_time = clock();
        qsort(key_table, curr_size, sizeof(key_row_t), cmp_key);
        curr_time = clock() - curr_time;
        sum_time2 += curr_time;

        curr_key_time = clock();

        for (int j = 0; j < curr_size; j++)
            if (&abon_table[key_table[j].table_ind]) 
                curr_time = 0;

        curr_key_time = clock() - curr_key_time;
        sum_key_time += curr_key_time;
    }
    
    sum_key_time = sum_key_time / ITERATIONS;
    sum_time2 = sum_time2 / ITERATIONS;
    long long unsigned time_qsort_key = sum_time2 / (CLOCKS_PER_SEC / 1000000LLU);
    long long unsigned time_qaccess_key = sum_key_time / (CLOCKS_PER_SEC / 1000000LLU);
    /***/
    
    /* Сортировка: qsort, таблица: исходная */
    /***/
    
    clock_t sum_time1 = 0;
    
    for (int i = 0; i < ITERATIONS; i++)
    {
        curr_time = clock();
        qsort(abon_table, curr_size, sizeof(abonent_t), cmp_abon);
        curr_time = clock() - curr_time;
        sum_time1 += curr_time;
        in = fopen(filename, "rt");
        fget_table(in, abon_table, key_table, curr_size);
        fclose(in);
    }

    sum_time1 = sum_time1 / ITERATIONS;
    long long unsigned time_qsort = sum_time1 / (CLOCKS_PER_SEC / 1000000LLU);
    /***/
    
    /* Сортировка: mysort (bubble), таблица: исходная */
    /***/
    clock_t sum_time3 = 0;
    
    for (int i = 0; i < ITERATIONS; i++)
    {
        curr_time = clock();
        mysort_abon_table(abon_table, curr_size);
        curr_time = clock() - curr_time;
        sum_time3 += curr_time;
        in = fopen(filename, "rt");
        fget_table(in, abon_table, key_table, curr_size);
        fclose(in);
    }

    sum_time3 = sum_time3 / ITERATIONS;
    long long unsigned time_mysort = sum_time3 / (CLOCKS_PER_SEC / 1000000LLU);
    /***/ 
    
    // Table
    printf("\n                   Table           Keys     Key access time\n\n");
    printf("qsort   :"); 
    printf("%15llu", time_qsort);
    printf("%15llu + %15llu\n", time_qsort_key, time_qaccess_key);
    printf("MYSORT  :"); 
    printf("%15llu", time_mysort);
    printf("%15llu + %15llu\n", time_mysort_key, time_myaccess_key);

    // Размер
    size_t size1 = sizeof(abonent_t) * curr_size, size2 = sizeof(key_row_t) * curr_size;
    printf("SIZE    : %14zu B", size1);
    printf("%13zu B\n\n", size2);
    
    printf("Память, занимаемая ключами (процент от общей таблицы): %.3f%%\n",
            ((double) size2 / size1) * 100);
    printf("\nВыигрыш qsort по сравнению с mysort (%%, без ключей): %.3f%%\n",
            (((double) time_mysort - (double) time_qsort) / time_mysort) * 100);
    printf("Выигрыш qsort по сравнению с mysort (%%, с ключами): %.3f%%\n", 
            (((double) time_mysort_key - (double) time_qsort_key) / time_mysort_key) * 100);
    printf("\nВыигрыш сорт. таблицы ключей по сравнению с сорт. исходной (%%, qsort): %.3f%%\n", 
            (((double) time_qsort - (double) time_qsort_key) / time_qsort) * 100);
    printf("Выигрыш сорт. таблицы ключей по сравнению с сорт. исходной (%%, mysort): %.3f%%\n\n",
            (((double) time_mysort - (double) time_mysort_key) / time_mysort) * 100);
}

void command_show_curr_week(void)
{
    if (curr_size == 0) { show_err(ERR_EMPTY); return; }

    printf("Введите дату (ДД[1-31] ММ[1-12] ГГГГ[1800-2500]): ");

    struct personal_status user_date = {2000, 1, 1};

    if (input_date(&user_date, stdin)) { show_err(ERR_DATA); return; }

    printf("БЛИЖАЙШИЕ Д.Р.:\n\n");
    output_table_header(stdout);

    for (int i = 0; i < curr_size; i++)
        if ((abon_table[i].status_code == PERSONAL) &&
            (is_inaweek_date(user_date, abon_table[i].status.personal)))
            output_abonent(stdout, abon_table[i], i, VERBOSE);
}
