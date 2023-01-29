#ifndef __TYPES_H__

#define __TYPES_H__

#include <stdint.h>
#include <stdlib.h>

#define LEN_MANTISSA 30 + 2
#define LEN_INT_STR 30 + 1 + 1 + 1
#define LEN_INT_STR_MIN 1
#define LEN_FLOAT_STR 39 + 2
#define LEN_FLOAT_STR_MIN 5
#define LEN_EXPONENT 5

#define MIN_EXP -99999
#define MAX_EXP +99999

// bool
#define TRUE 1
#define FALSE 0

// set_special_numb
#define OK_CODE 10
#define INF_CODE 1
#define NULL_CODE 0

// shift_mant
#define RIGHT 1
#define LEFT 0

// round_numb
#define BASE 10

// compare_mant
#define FIRST_LARGER 1
#define EQUAL 0
#define SECOND_LARGER -1

typedef uint8_t bool;

typedef uint8_t digit_t;

typedef struct long_float
{
    digit_t mantissa[LEN_MANTISSA];
    int exponent;
    bool mant_sign;
    bool is_null;
    bool is_inf;
} long_float_t;

typedef struct str_float
{
    size_t before_dot;
    size_t after_dot;
    size_t exp_len;
    bool exp_sign;
    bool mant_sign;
    bool has_dot;
} str_float_t;

#endif // #ifndef __STRUCT_H__
