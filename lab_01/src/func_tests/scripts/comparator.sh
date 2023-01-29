#!/bin/bash

# Compairing integers

#cd ../../

EXIT_SUCCESS=0
EXIT_FAILURE=1

file1="$1"
file2="$2"

reg_check="^Результат: \S*"

data1="$( grep -o "$reg_check" "$file1" )"
data2="$( grep -o "$reg_check" "$file2" )"

if [ "$data1" != "$data2" ]; then
    exit $EXIT_FAILURE
fi

exit $EXIT_SUCCESS

