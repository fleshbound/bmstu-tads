#include <stdio.h>
#include <stdlib.h>

#include "commands.h" 

void show_info(void)
{
    fprintf(stdout, "Программа предназначена для обработки данных об абонентах\n");  
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

        if (com_ind == COM_IND_FILL) command_file_fill_tables();
        if (com_ind == COM_IND_SHOW) command_show_abon_table();
        if (com_ind == COM_IND_SHOW_KEY) command_show_key_table();
        if (com_ind == COM_IND_EXIT) is_working = 0;
        if (com_ind == COM_IND_ADD) command_add_abon_by_user();
        if (com_ind == COM_IND_DEL) command_del_abon_by_value();
        if (com_ind == COM_IND_SORT_KEY) command_sort_key_table();
        if (com_ind == COM_IND_SORT_ABON) command_sort_abon_table();
        if (com_ind == COM_IND_KEYSORT_ABON) command_keysort_abon_table();
        if (com_ind == COM_IND_CLEAR) command_clear_tables();
        if (com_ind == COM_IND_SHOW_DIFF) command_show_diff_sorts();
        if (com_ind == COM_IND_CURR_WEEK) command_show_curr_week();
        if (com_ind == COM_IND_SHOW_MENU) command_show_menu();
        if (com_ind == COM_IND_SHOW_RULES) command_show_rules();

        if (com_ind != COM_IND_EXIT) com_ind = ERR_COM_IND;
    }

    show_success();
    return EXIT_SUCCESS;
}
