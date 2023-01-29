#ifndef __TYPES_H__

#define __TYPES_H__

#include <stdint.h>

#define LEN_SURNAME 20 + 2
#define LEN_NAME 20 + 2
#define LEN_ADDRESS 35 + 2
#define LEN_OFFICE 30 + 2
#define LEN_COMPANY 30 + 2
#define MAX_PHONE 19999999999
#define MIN_PHONE 14000000000

#define MAX_SIZE 3000
#define MIN_SIZE 40

typedef int bool;

/* Личный статус */
struct personal_status
{
    uint16_t year;
    uint8_t day;
    uint8_t month;
};

/* Служебный статус */
struct official_status
{
    char office[LEN_OFFICE];
    char company[LEN_COMPANY];
};

/* Статус абонента */
typedef union status
{
    struct official_status official;
    struct personal_status personal;
} status_t;

/* Код статуса */
#define PERSONAL 1
#define OFFICIAL 0

/* Информация об абоненте */
typedef struct abonent_info
{
    char surname[LEN_SURNAME]; // фамилия
    char name[LEN_NAME]; // имя
    uint64_t phone; // номер телефона
    char address[LEN_ADDRESS]; // адрес
    int status_code; // код статуса
    status_t status; // статус
} abonent_t;

typedef struct key_info
{
    int table_ind;
    char key[LEN_SURNAME];
} key_row_t;

typedef char key_t;

#endif // __TYPES_H__
