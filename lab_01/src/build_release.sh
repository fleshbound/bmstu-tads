#!/bin/bash

ERR_EXIST=1
 
c_files=$( find . -regextype posix-egrep -regex "\.*/[a-zA-Z_0-9]+\.c" )

if [ -z "$c_files" ]; then
    echo "$0: No .c files were found."

    exit $ERR_EXIST
fi

for file in $c_files; do
	gcc -std=c99 -Wall -Werror -Wextra -Wfloat-equal -Wfloat-conversion -Wvla -O3 -g0 -c "$file"
done

o_files=$( find . -regextype posix-egrep -regex "\.*/[a-zA-Z_0-9]+\.o" ! -name "main.o" | cut -f 2 -d '/' )
f=()
i=0
for file in $o_files; do
    f[i]="$file"
    i=$(( i + 1 ))
done

gcc -o app.exe main.o "${f[@]}" -lm

if [ -s app.exe ]; then
    echo "[$0] Release build was created"
    exit 0
else
    exit 1
fi

