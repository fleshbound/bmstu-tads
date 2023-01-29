#ifndef ERRORS_H
#define ERRORS_H

#define ERR_ARGS 1 
#define ERR_DATA 2
#define ERR_SIZE 3
#define ERR_OPEN 4
#define ERR_CLOSE 5
#define ERR_FULL_TABLE 6
#define ERR_EMPTY 7
#define ERR_EMPTYSTR 8

#define MSG_ERR_ARGS "Ошибка: Неверные аргументы командной строки\n"
#define MSG_ERR_DATA "Ошибка: Неверный формат данных\n"
#define MSG_ERR_SIZE "Ошибка: Неверный размер\n"
#define MSG_ERR_OPEN "Ошибка: Невозможно открыть файл\n"
#define MSG_ERR_CLOSE "Ошибка: Невозможно закрыть файл\n"
#define MSG_ERR_FULL_TABLE "Ошибка: Достигнуто макс. кол-во записей\n"
#define MSG_ERR_EMPTY "Ошибка: Таблица пуста\n"
#define MSG_ERR_EMPTYSTR "Ошибка: Пустой ввод\n"

void show_err(const int err_code);

#endif
