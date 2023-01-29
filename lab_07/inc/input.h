#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>

#include "avltree.h"
#include "bstree.h"
#include "hashtable.h"

#define LEN_FILENAME 255
#define LEN_COM 255

#define VERBOSE 1
#define QUIET 0

#define READ_COUNT 1

int input_command(void);
int input_filename(char *const name);
void clear_str(char *const str);
int input_key(key_t *new_key);
int finput_key(FILE *const f, key_t *const new_key, const int code);

tree_t fget_bstree(FILE *const f);
avltree_t fget_avltree(FILE *const f);
int fget_hashtable(FILE *const f, hash_t table);

size_t get_file_count(FILE *const f);
int input_comp(double *const comp);

#endif
