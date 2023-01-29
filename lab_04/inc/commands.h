#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#define MIN_COM_IND 0
#define MAX_COM_IND 15
#define ERR_COM_IND 100

#define COM_IND_FILL_FROM_FILE 1
#define COM_IND_SHOW 2
#define COM_IND_ADD 3
#define COM_IND_DEL 4
#define COM_IND_SORT 5
#define COM_IND_SORT_DIFF 6
#define COM_IND_CLEAR_ALL 7
#define COM_IND_SHOW_MENU 8
#define COM_IND_SHOW_RULES 9
#define COM_IND_EXIT 0

#define MSG_RULES  "\nПРАВИЛА ВВОДА:\n" \
                   "a) Все буквы являются латинскими, поведение при вводе русских неопределено!\n" \
                   "b) Элементы дерева - целые числа в пределах от -2147483648 до 2147483647\n" \
                   "c) Повторные узлы будут игнорироваться\n"

#define MSG_COM_GET "\nКоманда (8 - меню, 9 - правила): "

#define MSG_COM1 "\nДОСТУПНЫЕ КОМАНДЫ:\n1. Заполнить стеки из файла"
#define MSG_COM2 "2. Показать содержимое стеков"
#define MSG_COM3 "3. Добавить элемент..."
#define MSG_COM4 "4. Удалить элемент..."
#define MSG_COM5 "5. Выполнить сортировку с помощью 3-го стека"
#define MSG_COM6 "6. Выполнить сортировку с анализом"
#define MSG_COM7 "7. Очистить все"
#define MSG_COM8 "8. Вывести меню"
#define MSG_COM9 "9. Вывести правила ввода"
#define MSG_COM10 "10. Добавить элемент в тестовый списковый стек"
#define MSG_COM11 "11. Удалить элемент из тестового спискового стека"
#define MSG_COM12 "12. Добавить элемент в тестовый стек на основе массива"
#define MSG_COM13 "13. Удалить элемент из тестового стека на основе массива"
#define MSG_COM14 "14. Показать тестовые стеки"
#define MSG_COM15 "15. Очистить тестовые стеки"
#define MSG_COM0 "0. Выход"

#define EPS 1e-10
#define VAR_1 1
#define VAR_2 2

void command_show_menu(void);
void command_show_rules(void);

void command_fill_stacks_from_file(void);
void command_show_stacks(void);
void command_add(void);
void command_del(void);
void command_sort_data(void);
void command_sort_diff(void);
void command_clear_all(void);
void command_clear_test(void);
void command_show_test(void);
void command_arr_del_test(void);
void command_arr_add_test(void);
void command_del_test(void);
void command_add_test(void);

#endif
