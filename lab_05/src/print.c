#include "print.h"

void print_newline(void)
{
    putc('\n', stdout);
}

void print_hyphen_line(int repeat)
{
    while (repeat-- > 0)
        putc('-', stdout);
    putc('\n', stdout);
}
