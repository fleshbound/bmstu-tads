#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "commands.h" 

void show_info(void)
{
    fprintf(stdout, "Программа предназначена для обработки целых чисел с помощью ДДП, АВЛ-дерева, хеш-таблицы\n");  
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

        if (com_ind == COM_IND_EXIT) is_working = 0; 
        if (com_ind == COM_IND_CLEAR) command_clear_all();
        if (com_ind == COM_IND_SHOW_MENU) command_show_menu();
        if (com_ind == COM_IND_SHOW_RULES) command_show_rules();
        if (com_ind == COM_IND_SET_FILE) command_set_file();
        if (com_ind == COM_IND_SHOW_STRUCT) command_show_structures();
        if (com_ind == COM_IND_SHOW_GRAPH_MATRIX) command_show_graph_with_matrix();
        if (com_ind == COM_IND_SHOW_GRAPH_LIST) command_show_graph_with_list();
        if (com_ind == COM_IND_SEARCH) command_search_minsum_vertex();
        if (com_ind == COM_IND_FILL) command_fill_structures();
        if (com_ind == COM_IND_SHOW_EFF) command_show_efficiency();

        if (com_ind != COM_IND_EXIT) com_ind = ERR_COM_IND;
    }

    command_clear_all();
    show_success();
    return EXIT_SUCCESS;
}
