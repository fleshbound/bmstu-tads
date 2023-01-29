#!/bin/bash

files="*.o *.exe *.gcda *.gcno *.out *.gcov *.txt"
files_in_scripts="./func_tests/scripts/*.txt"

i=0

for file in $files; do
    if [ -f "$file" ]; then
        rm -f "$file"
        i=$(( i + 1 ))
    fi
done

for file in $files_in_scripts; do
    if [ -f "$file" ]; then
            rm -f "$file"
            i=$(( i + 1 ))
    fi
done

echo "[$0] All trash files were deleted ($i)"

exit 0
