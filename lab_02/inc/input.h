#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "types.h"
#include "errors.h"
/* #include "menu.h" */

#define SEPARATORS "|----------|------------------------|------------------------|-------------|-----------------------------------|----------|----------------------------------------------------------------|"
#define SEPARATORS_MINI "|----------|------------------------|" 

#define MSG_SURNAME "Введите фамилию: "
#define MSG_NAME "Введите имя: "
#define MSG_PHONE "Введите номер телефона: "
#define MSG_ADDRESS "Введите адрес: "
#define MSG_PERSONAL "Введите личный статус (ДД ММ ГГГГ): "
#define MSG_OFFICIAL "Введите офиц. статус (компания, должность через Enter): "
#define MSG_KEY_CODE "Введите код статуса (0 - официальный, 1 - личный): "
#define MSG_ERR "\nНекорректный ввод\nПопробуйте снова: "
#define MSG_SIZE "Введите кол-во записей (40 - 6000): "

#define VERBOSE 1
#define QUIET 0

#define FMT_PHONE "%lu"
#define FMT_YEAR "%hu"
#define FMT_DAY "%hhu"
#define FMT_MONTH "%hhu"
#define FMT_STATUS_CODE "%d"

#define LEN_FILENAME 256
#define OFFSET 0
#define SMAX_CODE (8 + OFFSET)
#define SMAX_SURNAME (OFFSET + LEN_SURNAME)
#define SMAX_NAME (OFFSET + LEN_NAME)
#define SMAX_PHONE (11 + OFFSET)
#define SMAX_STATUS (LEN_COMPANY + LEN_OFFICE - 2)
#define SMAX_ADDRESS LEN_ADDRESS 

int input_abonent(FILE *const f, abonent_t *abonent, const int fmt_code);
void output_abonent(FILE *const f, abonent_t abonent, const int ind, const int fmt_code);
void output_table_header(FILE *const f);
void clear_str(char *const str);
void clear_abonent(abonent_t *abonent);
int input_filename(char *const name);
int input_size(int *const size);
int input_surname(FILE *const f, char *const surname);
int input_personal(struct personal_status *status, FILE *const f);
int input_date(struct personal_status *status, FILE *const f);
void output_key_table_header(FILE *const f);
void output_key_row(FILE *const f, key_row_t key_row, const int fmt_code);

#endif
