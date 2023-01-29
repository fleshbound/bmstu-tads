#include <stdlib.h>
#include <stdint.h>

#include "types.h"
#include "errors.h"
#include "longio.h"
#include <stdio.h>
/*
 * Функция, выполняющая сравнение двух мантисс mant1 и mant2
 * Возвращает:
 * EQUAL - элементы мантисс совпадают
 * FIRST_LARGER - в первой мантиссе первый несовпадающий эл-т больше,
 * чем эл-т во второй
 * SECOND_LARGER - в ином случае
 */
int compare_mant(digit_t *const mant1, digit_t *const mant2)
{
    size_t i = 0;

    while ((mant1[i] == mant2[i]) && (i < LEN_MANTISSA))
        i++;

    if (i == LEN_MANTISSA)
        return EQUAL;

    return (mant1[i] >= mant2[i]) ? FIRST_LARGER : SECOND_LARGER;
}

/*
 * Функция, выполняющая поэлементное вычитание второй мантиссы
 * из первой (один раз)
 * Возвращает: ничего
 */
void diminute_mant(digit_t *const mant1, digit_t *const mant2)
{
    size_t j;

    for (size_t i = 0; i < LEN_MANTISSA; i++)
    {
        j = LEN_MANTISSA - i - 1;

        // Если очередная цифра числа1 больше цифры числа2
        if (mant1[j] >= mant2[j])
            mant1[j] = mant1[j] - mant2[j];
        else // Иначе
        {
            // Переход к более старшей цифре
            j--;

            // Пока более старшие цифры равны 0
            // а индекс не дошел до начала мантиссы
            // "делаем" нули девятками
            while ((j != 0) && (mant1[j] == 0))
                mant1[j--] = 9;
            
            // Когда дошли до ненулевой цифры
            // уменьшаем ее на 1
            if (mant1[j] > 0)
                mant1[j]--;

            // Возвращаемся к текущей цифре
            // и увеличиваем ее на 10,
            // вычитаем цифру второго числа
            j = LEN_MANTISSA - i - 1;
            mant1[j] = mant1[j] + 10 - mant2[j];
        }
    }
}

/*
 * Функция, получающая результат деления numerator на divisor
 * и помещающая этот результат в res.
 * Гарантируется, что numerator и divisor нормализованы.
 * Вовзращает:
 * EXIT_SUCCESS - ошибок нет
 * ERR_INF - обнаружено переполнение
 */
int divide_long_float(long_float_t numerator, long_float_t divisor, long_float_t *const res)
{
    if ((numerator.is_null) && (!divisor.is_null))
    {
        res->is_null = TRUE;
        return EXIT_SUCCESS;
    }

    if (divisor.is_null)
    {
        res->is_inf = TRUE;
        return ERR_ZERODIV;
    }

    if (divisor.is_inf)
    {
        res->is_null = TRUE;
        return EXIT_SUCCESS;   
    }
    
    // Порядок результата
    res->exponent = numerator.exponent - divisor.exponent;
    res->mant_sign = numerator.mant_sign ^ divisor.mant_sign;

    digit_t mant_num[LEN_MANTISSA] = { 0 },
            mant_div[LEN_MANTISSA] = { 0 };

    copy_mant(mant_num, numerator.mantissa);
    copy_mant(mant_div, divisor.mantissa);

    size_t digit_count = 0;

    // Если мантисса делимого больше, результат будет в виде 1. ...
    // следовательно, увеличиваем порядок для нормального вида
    if (compare_mant(mant_num, mant_div) != SECOND_LARGER)
        res->exponent++;
    else
        shift_mant(mant_div, RIGHT, 1);

    while ((digit_count < LEN_MANTISSA - 1) && (!is_null_mant(mant_num)))
    {
        size_t curr_res_digit = 0;
        // "Двигаем вправо" делитель, если делимое меньше
        if (compare_mant(mant_num, mant_div) == SECOND_LARGER)
            shift_mant(mant_div, RIGHT, 1);
        
        // Если можно поделить
        while (compare_mant(mant_num, mant_div) != SECOND_LARGER)
        {
            curr_res_digit++;
            diminute_mant(mant_num, mant_div);
        }
        
        //printf("[%lu] %lu | %d\n", digit_count, curr_res_digit, is_null_mant(mant_num));
        res->mantissa[digit_count++] = curr_res_digit;
            
        if (compare_mant(mant_num, mant_div) == SECOND_LARGER)
            shift_mant(mant_div, RIGHT, 1);
    }

    //for (size_t i = 0; i < LEN_MANTISSA; i++)
    //    printf("%d-", res->mantissa[i]);
    round_number(res);
    
    if (res->is_inf)
        return ERR_INF;

    return EXIT_SUCCESS;
}
