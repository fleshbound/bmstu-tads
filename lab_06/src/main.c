#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "commands.h" 

void show_info(void)
{
    fprintf(stdout, "Программа предназначена для обработки целых чисел с помощью дерева двоичного поиска\n");  
}

void show_success(void)
{
    printf("Работа успешно завершена\n");
}

int main(void)
{
    int com_ind = ERR_COM_IND, is_working = 1;
    show_info();
    command_show_rules();
    command_show_menu();

    while (is_working)
    {
        while (com_ind == ERR_COM_IND) { com_ind = input_command(); }

        if (com_ind == COM_IND_FILL) command_fill_tree();
        if (com_ind == COM_IND_EXPORT) command_export_tree();
        if (com_ind == COM_IND_EXIT) is_working = 0; 
        if (com_ind == COM_IND_ADD) command_add_node();
        if (com_ind == COM_IND_DEL) command_del_node();
        if (com_ind == COM_IND_SORT_TREE) command_tree_sort();
        if (com_ind == COM_IND_MYSORT) command_mysort();
        if (com_ind == COM_IND_CLEAR) command_clear_tree();
        if (com_ind == COM_IND_SHOW_DIFF) command_tree_lookup(QUIET);
        if (com_ind == COM_IND_SHOW_MENU) command_show_menu();
        if (com_ind == COM_IND_SHOW_RULES) command_show_rules();
        if (com_ind == COM_IND_SHOW) command_show_tree();
        if (com_ind == COM_IND_SEARCH) command_tree_search();
        if (com_ind == COM_IND_APPLY) command_tree_lookup(VERBOSE);
        if (com_ind == COM_IND_NODES_COUNT) command_nodes_count();
        if (com_ind == COM_IND_HEIGHT) command_get_height();

        if (com_ind != COM_IND_EXIT) com_ind = ERR_COM_IND;
    }

    command_end();
    show_success();
    return EXIT_SUCCESS;
}
