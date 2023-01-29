#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "menu.h"
#include "longio.h"

void show_menu(void)
{   
    fprintf(stdout, "%s%s%s", TITLE, RULES_INT, RULES_FLOAT);
}

void show_error(const int err_code)
{
    if (err_code == ERR_SIZE)
        fprintf(stderr, "%s", S_ERR_SIZE);

    if (err_code == ERR_FORMAT)
        fprintf(stderr, "%s", S_ERR_FORMAT);

    if (err_code == ERR_INF)
        fprintf(stderr, "%s", S_ERR_INF);

    if (err_code == ERR_ZERODIV)
        fprintf(stderr, "%s", S_ERR_ZERODIV);
}

void show_message(char *const message)
{
    if (message != NULL)
        printf("%s", message);
}

void show_result(long_float_t res)
{
    fprintf(stdout, "%s", S_RESULT);
    output_float(res);
}
