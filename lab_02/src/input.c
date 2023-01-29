#include "input.h"

int input_date(struct personal_status *status, FILE *const f)
{
    if (fscanf(f, FMT_DAY, &status->day) <= 0)
        return EXIT_FAILURE;
    fgetc(f);

    if ((status->day == 0) || (status->day > 31))
        return EXIT_FAILURE;

    if (fscanf(f, FMT_MONTH, &status->month) <= 0)
        return EXIT_FAILURE;
    fgetc(f);

    if ((status->month == 0) || (status->month > 12))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int input_personal(struct personal_status *status, FILE *const f)
{
    if (fscanf(f, FMT_DAY, &status->day) <= 0)
        return EXIT_FAILURE;
    fgetc(f);

    if ((status->day == 0) || (status->day > 31))
        return EXIT_FAILURE;

    if (fscanf(f, FMT_MONTH, &status->month) <= 0)
        return EXIT_FAILURE;
    fgetc(f);

    if ((status->month == 0) || (status->month > 12))
        return EXIT_FAILURE;

    if (fscanf(f, FMT_YEAR, &status->year) <= 0)
        return EXIT_FAILURE;
    fgetc(f);

    if ((status->year < 1800) || (status->year > 2500))
        return EXIT_FAILURE;
   
    return EXIT_SUCCESS;
}

int input_official(struct official_status *status, FILE *const f)
{
    if (fgets(status->company, LEN_COMPANY, f) == NULL)
        return ERR_DATA;

    clear_str(status->company);
    if (strlen(status->company) == 0)
        return ERR_EMPTYSTR;

    if (fgets(status->office, LEN_OFFICE, f) == NULL)
        return ERR_DATA;

    clear_str(status->office);
    if (strlen(status->office) == 0)
        return ERR_EMPTYSTR;

    return EXIT_SUCCESS;
}

void clear_str(char *const str)
{
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\n')
            str[i] = '\0';
}

void clear_abonent(abonent_t *abonent)
{
    clear_str(abonent->surname);
    clear_str(abonent->name);
    clear_str(abonent->address);
    
    if (abonent->status_code == OFFICIAL)
    {
        clear_str(abonent->status.official.company);
        clear_str(abonent->status.official.office);
    }
}

int input_surname(FILE *const f, char *const surname)
{
    fprintf(stdout, "%s", MSG_SURNAME);

    if (fgets(surname, LEN_SURNAME, f) == NULL)
        return EXIT_FAILURE;

    clear_str(surname);

    return EXIT_SUCCESS;
}

int input_abonent(FILE *const f, abonent_t *abonent, const int fmt_code)
{
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", "Ввод абонента\n");

    // фамилия
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_SURNAME);
    if (fgets(abonent->surname, LEN_SURNAME, f) == NULL)
    {
        //printf("фамилия\n");
        return ERR_DATA;
    }
    
    clear_str(abonent->surname);
    if (strlen(abonent->surname) == 0)
        return ERR_EMPTYSTR;

    // имя
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_NAME);
    if (fgets(abonent->name, LEN_NAME, f) == NULL)
    {
        //printf("имя\n");
        return ERR_DATA;
    }

    clear_str(abonent->name);
    if (strlen(abonent->name) == 0)
        return ERR_EMPTYSTR;

    // телефон
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_PHONE);
    if (fscanf(f, FMT_PHONE, &abonent->phone) <= 0)
    {
        //printf("phone\n");
        return ERR_DATA; 
    }
    fgetc(f);
    if ((abonent->phone < MIN_PHONE) || (abonent->phone > MAX_PHONE))
        return ERR_DATA;

    // адрес
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_ADDRESS);
    if (fgets(abonent->address, LEN_ADDRESS, f) == NULL)
    {
       // printf("address\n");
        return ERR_DATA;
    }

    clear_str(abonent->address);
    if (strlen(abonent->address) == 0)
        return ERR_EMPTYSTR;

    // код статуса
    if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_KEY_CODE);
    if (fscanf(f, FMT_STATUS_CODE, &abonent->status_code) <= 0)
    {
        //printf("statuscode\n");
        return ERR_DATA;
    }
    fgetc(f);
    if ((abonent->status_code != OFFICIAL) && (abonent->status_code != PERSONAL))
        return ERR_DATA;

    // статус личный
    if (abonent->status_code == PERSONAL)
    {
        if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_PERSONAL);
        if (input_personal(&abonent->status.personal, f) != EXIT_SUCCESS)
        {
           // printf("personal\n");
            return ERR_DATA;
        }
    }
   
    int rc = EXIT_SUCCESS;
    // статус служебный
    if (abonent->status_code == OFFICIAL)
    {
        if (fmt_code == VERBOSE) fprintf(stdout, "%s", MSG_OFFICIAL);
        rc = input_official(&abonent->status.official, f);
        //if (rc)
        //    printf("official\n");
    }

    return rc;
}

void output_table_header(FILE *const f)
{
    fprintf(f, "\n| %*s ", -SMAX_CODE, "Index");
    fprintf(f, "| %*s ", -SMAX_SURNAME, "Surname");
    fprintf(f, "| %*s ", -SMAX_NAME, "Name");
    fprintf(f, "| %*s ", -SMAX_PHONE, "Phone");
    fprintf(f, "| %*s ", -SMAX_ADDRESS, "Address");
    fprintf(f, "| %*s ", -SMAX_CODE, "Status");
    fprintf(f, "| %*s |\n", -SMAX_STATUS, "Birth date / Company: Job");
    fprintf(f, "%s\n", SEPARATORS);
}

int input_filename(char *const name)
{
    printf("Введите имя файла: ");
    
    if (fgets(name, LEN_FILENAME, stdin) == NULL)
        return EXIT_FAILURE;

    clear_str(name);

    return EXIT_SUCCESS;
}

int input_size(int *const size)
{
    printf("Введите кол-во записей для обработки (%d - %d): ", MIN_SIZE, MAX_SIZE);
    
    if (fscanf(stdin, "%d", size) <= 0)
        return EXIT_FAILURE;
    fgetc(stdin);

    if ((*size < MIN_SIZE) || (*size > MAX_SIZE))
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void output_abonent(FILE *const f, abonent_t abonent, const int ind, const int fmt_code)
{
    if (fmt_code == QUIET)
    {
        fprintf(f, "%s\n%s\n", abonent.surname, abonent.name);
        fprintf(f, FMT_PHONE"\n", abonent.phone);
        fprintf(f, "%s", abonent.address);
        fprintf(f, FMT_STATUS_CODE"\n", abonent.status_code);
        
        if (abonent.status_code == PERSONAL)
            fprintf(f, "%d %d %d\n", abonent.status.personal.day, abonent.status.personal.month, abonent.status.personal.year);
        
        if (abonent.status_code == OFFICIAL)
            fprintf(f, "%s\n%s\n", abonent.status.official.company, abonent.status.official.office);
    }
    
    int offset = 0;
    if (fmt_code == VERBOSE)
    {
        fprintf(f, "| %*d ", -SMAX_CODE, ind);
        fprintf(f, "| %*s ", -SMAX_SURNAME, abonent.surname);
        fprintf(f, "| %*s ", -SMAX_NAME, abonent.name);
        fprintf(f, "| %*ld ", -SMAX_PHONE, abonent.phone);
        fprintf(f, "| %*s ", -SMAX_ADDRESS, abonent.address);
        
        if (abonent.status_code == PERSONAL)
        {
            fprintf(f, "| %*s ", -SMAX_CODE, "Personal");
            offset = SMAX_STATUS - 6;
            fprintf(f, "| %*hhu %*hhu %*hu |\n", 2, abonent.status.personal.day, 2, abonent.status.personal.month, -offset, abonent.status.personal.year);
        }

        if (abonent.status_code == OFFICIAL)
        {
            fprintf(f, "| %*s ", -SMAX_CODE, "Official");
            fprintf(f, "| %*s: %*s |\n", -LEN_COMPANY, abonent.status.official.company, -LEN_OFFICE - 4, abonent.status.official.office);
        }
    }
}

void output_key_table_header(FILE *const f)
{
    fprintf(f, "\n| %*s ", -SMAX_CODE, "Index");
    fprintf(f, "| %*s |\n", -SMAX_SURNAME, "Surname");
    fprintf(f, "%s\n", SEPARATORS_MINI); 
}

void output_key_row(FILE *const f, key_row_t key_row, const int fmt_code)
{
    if (fmt_code == VERBOSE)
    {
        fprintf(f, "| %*d ", -SMAX_CODE, key_row.table_ind);
        fprintf(f, "| %*s |\n", -SMAX_SURNAME, key_row.key);
    }

    if (fmt_code == QUIET)
        fprintf(f, "%d %s", key_row.table_ind, key_row.key);
}
