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

stack_t fget_stack(FILE *const f)
{
    long unsigned curr_pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (get_int_count(f) == 0)
    {
        show_err(ERR_DATA);
        return NULL;
    }

    stack_t stack = create_stack(MAX_STACK_SIZE);

    if (stack == NULL)
    {
        show_err(ERR_MEM);
        return NULL;
    }

    int numb;

    while (fscanf(f, "%d", &numb) == READ_COUNT)
    {
        push(stack, numb);

        if (get_size(stack) == get_max_size(stack))
        {
            puts("Стек заполнен, запись прекращена");
            break;
        }
    }

    fseek(f, curr_pos, SEEK_SET);

    return stack;
}

size_t get_int_count(FILE *const f)
{
    long unsigned curr_pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    int tmp = 0;
    size_t count = 0;

    while (fscanf(f, "%d", &tmp) == READ_COUNT)
        count++;

    fseek(f, curr_pos, SEEK_SET);

    return count;
}

arr_stack_t fget_arr_stack(FILE *const f)
{
    long unsigned curr_pos = ftell(f);
    fseek(f, 0, SEEK_SET);

    size_t size = get_int_count(f);

    if (size == 0)
        return NULL;

    arr_stack_t stack = create_arr_stack(size);

    int numb;

    while (fscanf(f, "%d", &numb) == READ_COUNT)
    {
        arr_push(stack, numb);

        if (get_arr_size(stack) == MAX_STACK_SIZE)
        {
            puts("Стек заполнен, запись прекращена");
            break;
        }
    }

    fseek(f, curr_pos, SEEK_SET);

    return stack;
}
