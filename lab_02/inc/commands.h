#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "errors.h"
#include "table.h"
#include "types.h"
#include "input.h"

#define MIN_COM_IND 0
#define MAX_COM_IND 13
#define ERR_COM_IND 100

#define COM_IND_FILL 1
#define COM_IND_SHOW 2
#define COM_IND_SHOW_KEY 3
#define COM_IND_ADD 4
#define COM_IND_DEL 5
#define COM_IND_SORT_ABON 6
#define COM_IND_SORT_KEY 7
#define COM_IND_KEYSORT_ABON 8
#define COM_IND_SHOW_DIFF 9
#define COM_IND_CURR_WEEK 10
#define COM_IND_CLEAR 11
#define COM_IND_SHOW_MENU 12
#define COM_IND_SHOW_RULES 13
#define COM_IND_EXIT 0

#define MSG_RULES  "\nПРАВИЛА ВВОДА:\n" \
                   "a) Все буквы являются латинскими, поведение при вводе русских неопределено!\n" \
                   "b) Фамилия, Имя, Телефон, Адрес, КодСтатуса, Статус\n" \
                   "\t- Фамилия (21) - буквы/цифры\n" \
                   "\t- Имя (21) - буквы/цифры\n"\
                   "\t- НомерТелефона - 11 цифр, без разделителей!: [1,400,000,00,00; 1,999,999,99,99]\n"\
                   "\t- Адрес (36) - буквы/цифры\n"\
                   "\t- КодСтатуса - 0 (офиц.) или 1 (личн.)\n"\
                   "\t-- Статус-Официальный - Компания (31), Должность (31)\n"\
                   "\t-- Статус-Личный - ДатаРождения (День, Месяц, Год)\n"
      /*             "c) Отсутствуют проверки на:\n"\
                   "\t- Переполнение буфера\n"\
                   "\t- Некорректность даты в рамках кол-ва дней/месяцев/годов\n"\
                   "\t- Ввод номера, как строки - считается некорректным вводом\n"\
                   "d) При некорректном вводе абонента, ввод данных начинается заново\n"\
                   "e) При некорректном вводе Телефона поведение неопределено\n"\
                   "f) Регистр имеет значение!\n"*/

#define MSG_COM_GET "\nКоманда (12 - меню, 13 - правила): "

#define MSG_COM1 "\nДОСТУПНЫЕ КОМАНДЫ:\n1. Заполнить таблицу абонентов из файла"
#define MSG_COM2 "2. Вывести таблицу с данными об абонентах"
#define MSG_COM3 "3. Вывести таблицу с ключами (фамилиями абонентов)"
#define MSG_COM4 "4. Добавить абонента в конец таблицы"
#define MSG_COM5 "5. Удалить абонента, фамилия которого совпадает с введенной"
#define MSG_COM6 "6. Отсортировать и вывести таблицу абонентов"
#define MSG_COM7 "7. Отсортировать и вывести таблицу ключей (фамилий)"
#define MSG_COM8 "8. Отсортировать таблицу абонентов с помощью таблицы ключей"
#define MSG_COM9 "9. Вывести результаты сортировки пузырьком и быстрым алгоритмом (qsort)"
#define MSG_COM10 "10. Вывести список абонентов, чей Д.Р. -- в ближайшую неделю от введенной даты"
#define MSG_COM11 "11. Очистить таблицы"
#define MSG_COM12 "12. Вывести меню"
#define MSG_COM13 "13. Вывести правила ввода"
#define MSG_COM0 "0. Выход"

int input_command(void);
void command_file_fill_tables(void);
void command_add_abon_by_user(void);
void command_show_abon_table(void);
void command_show_key_table(void);
void command_del_abon_by_value(void);
void command_sort_key_table(void);
void command_sort_abon_table(void);
void command_keysort_abon_table(void);
void command_show_diff_sorts(void);
void command_show_curr_week(void);
void command_show_menu(void);
void command_clear_tables(void);
void command_show_rules(void);

#endif
