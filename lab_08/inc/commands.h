#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#define MIN_COM_IND 0
#define MAX_COM_IND 10
#define ERR_COM_IND 100

#define COM_IND_SET_FILE 1
#define COM_IND_FILL 2
#define COM_IND_SHOW_STRUCT 3
#define COM_IND_SHOW_GRAPH_MATRIX 4
#define COM_IND_SHOW_GRAPH_LIST 5
#define COM_IND_SEARCH 6
#define COM_IND_SHOW_EFF 7
#define COM_IND_SHOW_MENU 8
#define COM_IND_SHOW_RULES 9
#define COM_IND_CLEAR 10
#define COM_IND_EXIT 0

#define MSG_RULES  "\nПРАВИЛА ВВОДА:\n" \
                   "a) Все буквы являются латинскими, поведение при вводе русских неопределено!\n" \
                   "b) Для каждой пары городов есть двухсторонняя дорога, длина которой положительна\n" \
                   "c) Ввод матрицы смежности и списков ребер осуществляется через заполнение соответствующих файлов\n"

#define MSG_COM_GET "\nКоманда (8 - меню, 9 - правила): "

#define MSG_COM1 "\nДОСТУПНЫЕ КОМАНДЫ:\n1. Задать файл для обработки"
#define MSG_COM2 "2. Заполнить из файла таблицу стоимостей, список"
#define MSG_COM3 "3. Вывести на экран таблицу стоимостей, список"
#define MSG_COM4 "4. Показать граф (по таблице)"
#define MSG_COM5 "5. Показать граф (по списку)"
#define MSG_COM6 "6. Найти город с минимальной суммой кратч. путей до остальных городов"
#define MSG_COM7 "7. Выполнить сравнение затрат"
#define MSG_COM8 "8. Вывести меню"
#define MSG_COM9 "9. Вывести правила ввода"
#define MSG_COM10 "10. Очистить все"
#define MSG_COM0 "0. Выход"

#define EPS 1e-10

void command_show_menu(void);
void command_show_rules(void);

void command_set_file(void);
void command_fill_structures(void);
void command_show_structures(void);
void command_show_graph_with_matrix(void);
void command_show_graph_with_list(void);
void command_search_minsum_vertex(void);
void command_show_efficiency(void);
void command_clear_all(void);


#endif
