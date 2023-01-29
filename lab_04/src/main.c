#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "commands.h" 

void show_info(void)
{
    fprintf(stdout, "Программа предназначена для обработки целых чисел с помощью АТД -- стека\n");  
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
        if (com_ind == COM_IND_DEL) command_del();
        if (com_ind == COM_IND_SHOW) command_show_stacks();
        if (com_ind == COM_IND_FILL_FROM_FILE) command_fill_stacks_from_file();
        if (com_ind == COM_IND_ADD) command_add();
        if (com_ind == COM_IND_CLEAR_ALL) command_clear_all();
        if (com_ind == COM_IND_SORT_DIFF) command_sort_diff();
        if (com_ind == COM_IND_SORT) command_sort_data();
        if (com_ind == 10) command_add_test();
        if (com_ind == 11) command_del_test();
        if (com_ind == 12) command_arr_add_test();
        if (com_ind == 13) command_arr_del_test();
        if (com_ind == 14) command_show_test();
        if (com_ind == 15) command_clear_test();

        if (com_ind != COM_IND_EXIT) com_ind = ERR_COM_IND;
    }

    command_clear_all();
    show_success();
    return EXIT_SUCCESS;
}
