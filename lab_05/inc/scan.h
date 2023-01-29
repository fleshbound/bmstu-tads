#ifndef __SCAN_H__
#define __SCAN_H__

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "print.h"

int flush_input(void);
int scan_integer(int *value, int min, int max);
int scan_menu(int *key);

#endif  // __SCAN_H__
