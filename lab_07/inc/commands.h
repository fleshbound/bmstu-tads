#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#define MIN_COM_IND 0
#define MAX_COM_IND 15
#define ERR_COM_IND 100

#define COM_IND_SET_FILE 1
#define COM_IND_SHOW_TREES 2
#define COM_IND_SHOW_HASH 3
#define COM_IND_DIFF 4
#define COM_IND_NEW 5
#define COM_IND_ADD 6
#define COM_IND_SEARCH 7
#define COM_IND_CLEAR_ALL 8
#define COM_IND_SWITCH 9
#define COM_IND_SHOW_MENU 10
#define COM_IND_SHOW_RULES 11
#define COM_IND_RESTR 12
#define COM_IND_EXIT 0

#define MSG_RULES  "\nПРАВИЛА ВВОДА:\n" \
                   "a) Все буквы являются латинскими, поведение при вводе русских неопределено!\n" \
                   "b) Элементы дерева - целые числа в пределах от -2147483648 до 2147483647\n" \
                   "c) Повторные узлы будут игнорироваться\n"

#define MSG_COM_GET "\nКоманда (10 - меню, 11 - правила): "

#define MSG_COM1 "\nДОСТУПНЫЕ КОМАНДЫ:\n1. Задать файл для обработки"
#define MSG_COM2 "2. Вывести на экран ДДП, АВЛ-дерево"
#define MSG_COM3 "3. Вывести на экран хеш-таблицу"
#define MSG_COM4 "4. Выполнить поиск с анализом"
#define MSG_COM5 "5. Создать новый файл с данными"
#define MSG_COM6 "6. Добавить новый элемент"
#define MSG_COM7 "7. Выполнить поиск в структурах"
#define MSG_COM8 "8. Очистить все"
#define MSG_COM9 "9. Изменить адресацию хеш-таблицы"
#define MSG_COM10 "10. Вывести меню"
#define MSG_COM11 "11. Вывести правила ввода"
#define MSG_COM12 "12. Реструктурировать таблицу"
#define MSG_COM0 "0. Выход"

#define EPS 1e-10

void command_show_menu(void);
void command_show_rules(void);

void command_set_file(void);
int command_set_filename(void);
int command_fill_bstree(void);
int command_fill_avltree(void);
int command_fill_hashtable(const int addr_code);
void command_show_hashtable(void);
void command_add_key(void);
void command_show_trees(void);
void command_new_file(void);
void command_switch_hashtable(void);
void command_search_key(void);
void command_clear_all(void);
void command_search_results(void);
void command_restruct(void);

#endif
