#ifndef ERRORS_H
#define ERRORS_H

#define ERR_ARGS 1 
#define ERR_DATA 2
#define ERR_SIZE 3
#define ERR_OPEN 4
#define ERR_CLOSE 5
#define ERR_EMPTY 7
#define ERR_EMPTYSTR 8
#define ERR_MEM 9
#define ERR_FULL_HASH 10
#define ERR_FILE 11
#define ERR_EMPTY_HASH 12
#define ERR_EXISTING_KEY 13

#define MSG_ERR_ARGS "Ошибка: Неверные аргументы командной строки\n"
#define MSG_ERR_DATA "Ошибка: Неверный формат данных\n"
#define MSG_ERR_SIZE "Ошибка: Неверный размер\n"
#define MSG_ERR_OPEN "Ошибка: Невозможно открыть файл\n"
#define MSG_ERR_CLOSE "Ошибка: Невозможно закрыть файл\n"
#define MSG_ERR_EMPTY "Ошибка: Дерево пусто\n"
#define MSG_ERR_EMPTYSTR "Ошибка: Пустой ввод\n"
#define MSG_ERR_MEM "Ошибка: Не удалось выделить память\n"
#define MSG_ERR_FULL_HASH "Ошибка: Переполнение хеш-таблицы\n"
#define MSG_ERR_FILE "Ошибка: Не выбран файл\n"
#define MSG_ERR_EMPTY_HASH "Ошибка: Хеш-таблица пуста\n"
#define MSG_ERR_EXISTING_KEY "Ошибка: Элемент уже присутствует в структурах\n"

void show_err(const int err_code);

#endif
