#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "errors.h"
#include "input.h"

void clear_str(char *const str)
{
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\n')
            str[i] = '\0';
}

int input_filename(char *const name)
{
    printf("Введите имя файла: ");
    
    if (fgets(name, LEN_FILENAME, stdin) == NULL)
        return EXIT_FAILURE;

    clear_str(name);

    return EXIT_SUCCESS;
}

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

int finput_node(FILE *const f, int *const new_key, const int code)
{
    if (code == VERBOSE) fprintf(stdout, "Введите узел: ");

    if (fscanf(f, "%d", new_key) != READ_COUNT)
        return EXIT_FAILURE;

    fgetc(f);

    return EXIT_SUCCESS;
}

int input_node(int *new_key)
{
    return finput_node(stdin, new_key, VERBOSE);
}
