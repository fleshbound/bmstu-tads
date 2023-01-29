#include <stdlib.h>
#include <stdio.h>

#include "menu.h"
#include "longproc.h"
#include "longio.h"

int main(void)
{
    int err_code = EXIT_SUCCESS;
    
    show_menu();
    
    long_float_t int_numb;
    err_code = input_int(&int_numb);

    if (err_code)
    {
        show_error(err_code);
        return err_code;
    }

    long_float_t float_numb;
    err_code = input_float(&float_numb);

    if (err_code)
    {
        show_error(err_code);
        return err_code;
    }

//    output_float(float_numb);

    long_float_t result;
    init_number(&result);
    err_code = divide_long_float(int_numb, float_numb, &result);

    if (err_code)
    {
        show_error(err_code);
        return err_code;
    }

    show_result(result);

    return EXIT_SUCCESS;
}
