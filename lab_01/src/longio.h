#ifndef __LONGIO_H__

#define __LONGIO_H__

#include "types.h"

int input_float(long_float_t *const number);

void output_float(long_float_t number);

int input_int(long_float_t *const number);

size_t normalize_mant(digit_t *const mant);

void shift_mant(digit_t *const mant, const bool dir, const size_t offset);

void copy_mant(digit_t *const dest, digit_t *const src);

bool is_null_mant(digit_t *const mant);

void round_number(long_float_t *const num);

void init_number(long_float_t *const n);

#endif // __LONGIO_H__
