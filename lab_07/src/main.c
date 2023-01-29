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
        /* if (com_ind == COM_IND_CLEAR) command_clear_all(); */
        if (com_ind == COM_IND_SHOW_MENU) command_show_menu();
        if (com_ind == COM_IND_SHOW_RULES) command_show_rules();
//////////
        if (com_ind == COM_IND_SHOW_TREES) command_show_trees();
        if (com_ind == COM_IND_SET_FILE) command_set_file();
        if (com_ind == COM_IND_SHOW_HASH) command_show_hashtable();
        if (com_ind == COM_IND_ADD) command_add_key();
        if (com_ind == COM_IND_NEW) command_new_file();
        if (com_ind == COM_IND_SWITCH) command_switch_hashtable();
        if (com_ind == COM_IND_SEARCH) command_search_key();
        if (com_ind == COM_IND_CLEAR_ALL) command_clear_all();
        if (com_ind == COM_IND_DIFF) command_search_results();
        if (com_ind == COM_IND_RESTR) command_restruct();

        if (com_ind != COM_IND_EXIT) com_ind = ERR_COM_IND;
    }

    /* command_end(); */
    show_success();
    return EXIT_SUCCESS;
}
