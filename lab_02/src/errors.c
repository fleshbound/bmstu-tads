#include "errors.h"
#include <stdio.h>

void show_err(const int err_code)
{
    if (err_code == ERR_ARGS) fprintf(stderr, MSG_ERR_ARGS);
    if (err_code == ERR_DATA) fprintf(stderr, MSG_ERR_DATA);
    if (err_code == ERR_SIZE) fprintf(stderr, MSG_ERR_SIZE);
    if (err_code == ERR_OPEN) fprintf(stderr, MSG_ERR_OPEN);
    if (err_code == ERR_CLOSE) fprintf(stderr, MSG_ERR_CLOSE);
    if (err_code == ERR_FULL_TABLE) fprintf(stderr, MSG_ERR_FULL_TABLE);
    if (err_code == ERR_EMPTY) fprintf(stderr, MSG_ERR_EMPTY);
    if (err_code == ERR_EMPTYSTR) fprintf(stderr, MSG_ERR_EMPTYSTR);
}
