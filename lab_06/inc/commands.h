#ifndef COMMANDS_H
#define COMMANDS_H

#define MIN_COM_IND 0
#define MAX_COM_IND 15
#define ERR_COM_IND 100

#define COM_IND_FILL 1
#define COM_IND_EXPORT 2
#define COM_IND_ADD 3
#define COM_IND_DEL 4
#define COM_IND_SEARCH 5
#define COM_IND_APPLY 6
#define COM_IND_SORT_TREE 7
#define COM_IND_MYSORT 8
#define COM_IND_SHOW_DIFF 9
#define COM_IND_CLEAR 10
#define COM_IND_SHOW_MENU 11
#define COM_IND_SHOW_RULES 12
#define COM_IND_NODES_COUNT 13
#define COM_IND_SHOW 14
#define COM_IND_HEIGHT 15
#define COM_IND_EXIT 0

#define MSG_RULES  "\nПРАВИЛА ВВОДА:\n" \
                   "a) Все буквы являются латинскими, поведение при вводе русских неопределено!\n" \
                   "b) Элементы дерева - целые числа в пределах от -2147483648 до 2147483647\n" \
                   "c) Повторные узлы будут игнорироваться\n"

#define MSG_COM_GET "\nКоманда (11 - меню, 12 - правила): "

#define MSG_COM1 "\nДОСТУПНЫЕ КОМАНДЫ:\n1. Загрузить дерево из файла"
#define MSG_COM2 "2. Экспортировать дерево в изображение (dot)"
#define MSG_COM3 "3. Добавить узел в дерево / файл, сравнение времени"
#define MSG_COM4 "4. Удалить узел из дерева"
#define MSG_COM5 "5. Выполнить поиск узла"
#define MSG_COM6 "6. Выполнить обход дерева"
#define MSG_COM7 "7. Отсортировать числа из файла с помощью дерева"
#define MSG_COM8 "8. Отсортировать числа из файла с помощью метода 'qsort'"
#define MSG_COM9 "9. Вывести время разных вариантов обхода"
#define MSG_COM10 "10. Очистить дерево"
#define MSG_COM11 "11. Вывести меню"
#define MSG_COM12 "12. Вывести правила ввода"
#define MSG_COM13 "13. Определить кол-во узлов на каждом уровне дерева"
#define MSG_COM14 "14. Перевести текущий .dot файл в .png и открыть его"
#define MSG_COM15 "15. Получить высоту загруженного дерева"
#define MSG_COM0 "0. Выход"

#define NEW_TREE 1
#define EXIST_TREE 2

void command_show_menu(void);
void command_show_rules(void);

void command_fill_tree(void);
void command_export_tree(void);
void command_add_node(void);
void command_del_node(void);
void command_tree_search(void);
void command_tree_lookup(const int code);
void command_tree_sort(void);
void command_mysort(void);
void command_clear_tree(void);
void command_nodes_count(void);
void command_show_tree(void);
void command_end(void);
void command_get_height(void);

#endif
