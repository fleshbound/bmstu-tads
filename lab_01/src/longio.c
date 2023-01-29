#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "errors.h"
#include "types.h"
#include "menu.h"
#include "longio.h"

// Проверка на корректность целого числа в строке
bool is_correct_int(char *const str)
{
    size_t len = strlen(str), sign_offset = 1;

    if (len < LEN_INT_STR_MIN)
        return FALSE;

    if ((str[0] != '+') && (str[0] != '-'))
        sign_offset = 0;

    for (size_t i = sign_offset; i < len; i++)
        if (!isdigit(str[i]))
            return FALSE;
    
    return TRUE;
}

// Очищение строки от символовпереноса строки
void clean_str(char *const str)
{
    for (size_t i = 0; i < strlen(str); i++)
        if (str[i] == '\n')
            str[i] = '\0';
}

// Сдвиг мантиссы в заданном направлении на offset элементов
void shift_mant(digit_t *const mant, const bool dir, const size_t offset)
{
    if (dir == RIGHT)
    {
        for (size_t i = LEN_MANTISSA - 1; i >= offset; i--)
            mant[i] = mant[i - 1];

        for (size_t i = 0; i < offset; i++)
            mant[i] = 0;
    }
    else if (dir == LEFT)
    {
        for (size_t i = offset; (i < LEN_MANTISSA); i++)
            mant[i - offset] = mant[i];

        for (size_t i = LEN_MANTISSA - offset; i < LEN_MANTISSA; i++)
            mant[i] = 0;
    }
}

// Удаление нулей "слева" из мантиссы
// Возвращает количество удаленных элементов
size_t normalize_mant(digit_t *const mant)
{
    size_t q = 0;

    while ((q < LEN_MANTISSA) && (mant[q] == 0))
        q++;

    if ((q != LEN_MANTISSA) && (q != 0))
        shift_mant(mant, LEFT, q);

    return q;
}

// Нормализация числа (мантиссы и увеличение порядка)
void normalize_number(long_float_t *const number)
{
    size_t exp_offset = normalize_mant(number->mantissa);
    number->exponent = number->exponent - exp_offset;
}

// Проверка мантиссы на нулевую
bool is_null_mant(digit_t *const mant)
{
    for (size_t i = 0; i < LEN_MANTISSA; i++)
        if (mant[i] != 0)
            return FALSE;

    return TRUE;
}

// Установка флагов нуля и переполнения
void set_special_numb(long_float_t *const number)
{
    if (number->exponent > MAX_EXP)
        number->is_inf = TRUE;

    if (number->exponent < MIN_EXP)
        number->is_null = TRUE;

    if (is_null_mant(number->mantissa))
        number->is_null = TRUE;
}

// Округление числа (с установкой флагов)
void round_number(long_float_t *const num)
{
    if (num->mantissa[LEN_MANTISSA - 1] >= 5)
    {
        num->mantissa[LEN_MANTISSA - 1] = 0;
        num->mantissa[LEN_MANTISSA - 2]++;
    }
    
    if (num->mantissa[LEN_MANTISSA - 2] >= 5)
    {
        num->mantissa[LEN_MANTISSA - 2] = 0;

        digit_t addition = 1, new_digit = 0;

        for (int i = LEN_MANTISSA - 3; i >= 0; i--)
        {
            new_digit = num->mantissa[i] + addition;
            num->mantissa[i] = new_digit % BASE;
            addition = new_digit / BASE;
        }

        if (addition != 0)
        {
            num->exponent++;
            shift_mant(num->mantissa, RIGHT, 1);
            num->mantissa[0] = addition;
        }
    }

    set_special_numb(num);
}

// Перевод строки с целым числом в структуру
int convert_int(char *const num_str, long_float_t *const number)
{
    size_t len = strlen(num_str), sign_offset = 1;
   
    if (len > LEN_MANTISSA - 2)
        len = LEN_MANTISSA - 2;

    number->exponent = len - 1;

    if (num_str[0] == '-')
        number->mant_sign = TRUE;
    else if (num_str[0] != '+')
    {
        sign_offset = 0;
        number->exponent = len;
    }

    for (size_t i = sign_offset; i < len; i++)
        number->mantissa[i - sign_offset] = num_str[i] - '0';

    normalize_number(number);
    set_special_numb(number);

    if (number->is_inf)
        return ERR_SIZE;

    return EXIT_SUCCESS;
}

// Копирование элементов мантиссы src в мантиссу dest
void copy_mant(digit_t *const dest, digit_t *const src)
{
    for (size_t i = 0; i < LEN_MANTISSA; i++)
        dest[i] = src[i];
}

// Копирование числа src в число dest
void copy_number(long_float_t *const dest, long_float_t src)
{
    dest->is_inf = src.is_inf;
    dest->is_null = src.is_null;
    dest->exponent = src.exponent;
    copy_mant(dest->mantissa, src.mantissa);
}

// Инициализация числа (обнуление)
void init_number(long_float_t *const n)
{
    for (size_t i = 0; i < LEN_MANTISSA; i++)
        n->mantissa[i] = 0;

    n->exponent = 0;
    n->mant_sign = FALSE;
    n->is_null = FALSE;
    n->is_inf = FALSE;
}

// Ввод числа, его перевод
int input_int(long_float_t *const number)
{
    show_message(INPUT_INT);
    init_number(number);    
    char int_str[LEN_INT_STR] = "";
    
    if (fgets(int_str, LEN_INT_STR, stdin) == NULL)
        return ERR_SIZE;
    
    clean_str(int_str);

    if (!is_correct_int(int_str))
        return ERR_FORMAT;

    int err_code = convert_int(int_str, number);

    if (err_code)
        return err_code;

    return EXIT_SUCCESS;
}

// Проверка строки на соответствие корректному виду порядка
// и заполнение вспомогательной структуры
bool is_exppart(char *const str, char *const end, str_float_t *const float_chars)
{
    char *beg = str;

    if (beg > end)
        return TRUE;

    if ((*beg != 'e') && (*beg != 'E'))
        return FALSE;

    beg = beg + 1;

    if (beg > end)
        return FALSE;

    if ((*beg == '+') || (*beg == '-'))
    {
        float_chars->exp_sign = TRUE;
        beg = beg + 1;
    }

    int has_digit = 0;

    while (beg <= end)
    {
        if (!isdigit(*beg))
            return FALSE;
        else
        {
            float_chars->exp_len++;
            has_digit = 1;
        }

        beg = beg + 1;
    }

    if (!has_digit)
        return FALSE;

    return TRUE;
}

// Проверка строки на корректное действительное число
// и заполнение вспомогательной структуры
bool is_correct_float(char *const str, str_float_t *const float_chars)
{
    if (strlen(str) == 0)
        return FALSE;

    if ((strlen(str) == 1) && (!isdigit(str[0])))
        return FALSE;

    size_t i = 0;
    char correct_first[] = "0123456789.-+";

    if (strchr(correct_first, str[i]) == NULL)
        return FALSE;

    if ((str[i] == '+') || (str[i] == '-'))
    {
        float_chars->mant_sign = TRUE;
        i++;
    }

    int has_dot = 0;

    if (str[i] == '.')
    {
        i++;
        has_dot = 1;
        float_chars->has_dot = TRUE;
    }

    int has_digit = 0;

    while (isdigit(str[i]))
    {
        has_digit = 1;
        i++;

        if (has_dot)
            float_chars->after_dot++;
        else
            float_chars->before_dot++;
    }

    if ((str[i] == '.') && (!has_dot))
    {
        i++;
        float_chars->has_dot = TRUE;

        while (isdigit(str[i]))
        {
            i++;
            float_chars->after_dot++;
            has_digit = 1;
        }
    }

    if (!has_digit)
        return FALSE;

    if (!is_exppart(str + i, str + strlen(str) - 1, float_chars))
        return FALSE;

    if ((float_chars->after_dot + float_chars->before_dot > LEN_MANTISSA - 2) 
            || (float_chars->exp_len > LEN_EXPONENT))
        return FALSE;

    return TRUE;
}

// Перевод строки с действительным числом в структуру
// с помощью структуры с информацией о строке
int convert_float(char *const num_str, long_float_t *const number, str_float_t *const float_chars)
{
    size_t s_offset = 0, d_offset = 0, es_offset = 0;

    if (float_chars->mant_sign)
        s_offset = 1;

    if (float_chars->has_dot)
        d_offset = 1;

    if (float_chars->exp_sign)
        es_offset = 1;

    size_t dot_pos = s_offset + float_chars->before_dot;
    size_t e_pos = dot_pos + float_chars->after_dot + d_offset;
    size_t edigit_pos = e_pos + es_offset + 1;

    if ((s_offset) && (num_str[0] == '-'))
        number->mant_sign = TRUE;

    size_t count = 0;

    // добавление цифр до точки
    for (size_t i = s_offset; i < dot_pos; i++)
        number->mantissa[count++] = num_str[i] - '0';

    // добавление цифр после точки при наличии
    for (size_t i = dot_pos + 1; (float_chars->after_dot) && (i < e_pos); i++)
        number->mantissa[count++] = num_str[i] - '0';

    int j = 1;

    for (size_t i = edigit_pos + float_chars->exp_len - 1; (float_chars->exp_len) && (i >= edigit_pos); i--)
    {
        number->exponent = number->exponent + j * (num_str[i] - '0');
        j = j * 10;
    }

    if ((es_offset) && (num_str[edigit_pos - 1] == '-'))
        number->exponent = -number->exponent;

    number->exponent = number->exponent + float_chars->before_dot;

    normalize_number(number);

    return EXIT_SUCCESS;
}

// Инициализация вспомогательной структуры
void init_chars(str_float_t *const chars)
{
    chars->before_dot = 0;
    chars->after_dot = 0;
    chars->exp_len = 0;
    chars->exp_sign = FALSE;
    chars->mant_sign = FALSE;
    chars->has_dot = FALSE;
}

// Ввод действительного числа, перевод в структуру
int input_float(long_float_t *const number)
{
    show_message(INPUT_FLOAT);
    init_number(number);
    char float_str[LEN_FLOAT_STR] = "";

    if (fgets(float_str, LEN_FLOAT_STR, stdin) == NULL)
        return ERR_SIZE;

    clean_str(float_str);
    
    str_float_t float_chars;
    init_chars(&float_chars);

    if (!is_correct_float(float_str, &float_chars))
        return ERR_FORMAT;

    int err_code = convert_float(float_str, number, &float_chars);

    if (err_code)
        return err_code;

    set_special_numb(number);
    number->is_null = FALSE;

    if (is_null_mant(number->mantissa))
        number->is_null = TRUE;

    return EXIT_SUCCESS;
}

void output_float(long_float_t number)
{
    if (!number.is_null)
    {
        fprintf(stdout, "%s0.", (number.mant_sign) ? "-" : "+");

        size_t end = LEN_MANTISSA - 3;

        while (number.mantissa[end] == 0)
            end--;

        for (size_t i = 0; i <= end; i++)
            fprintf(stdout, "%d", number.mantissa[i]);

        fprintf(stdout, "E%+d\n", number.exponent);
    }
    else
        fprintf(stdout, "0\n");
}
