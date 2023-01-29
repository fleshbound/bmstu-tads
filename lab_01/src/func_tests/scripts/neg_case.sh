#!/bin/bash

EXIT_SUCCESS=0
ERROR_ANSWER=1
ERROR_MEMORY=2
EXIT_SUCCESS_VALGRIND=3
ERROR_MEMORY_ANSWER=4

file_stream_in="$1"
file_app_args="$2"

#cd ../../

if [ $# -eq 3 ]; then
    file_stream_out_except="$2"
    file_app_args="$3"
fi

declare -a app_args=()
if [ ! \( -z "$file_app_args"  \) ]; then
    argstr=$( grep -v "^$" "$file_app_args" )
    app_args=($argstr)
    #echo "${app_args[@]}"
fi

error_memory=0
if [ "$FLAG_VAL" = "1" ]; then
    {
        valgrind --tool=memcheck --log-file=log.txt --quiet ./app.exe "${app_args[@]}" < "$file_stream_in"
        res_code=$?
    } > /dev/null 2>&1

    if [ -s "log.txt" ]; then
        error_memory=1
    fi
else
    {
        ./app.exe "${app_args[@]}" < "$file_stream_in"
        res_code=$?
    } > /dev/null 2>&1
fi

error_answer=0
if [ -z "$file_stream_out_except" ]; then
    if [ $res_code -eq 0 ]; then      
        error_answer=1
    fi
else    
    data=$( grep -o "[0-9][0-9]*" "$file_stream_out_except" )

    if [ "$res_code" != "$data" ]; then
        error_answer=0
    fi
fi

if [ $error_answer -eq 0 ] && [ $error_memory -eq 0 ] && [ "$FLAG_VAL" = "1" ]; then
    exit $EXIT_SUCCESS_VALGRIND
elif [ $error_answer -eq 0 ] && [ $error_memory -eq 0 ]; then
    exit $EXIT_SUCCESS
elif [ $error_answer -ne 0 ] && [ $error_memory -eq 0 ]; then
    exit $ERROR_ANSWER
elif [ $error_answer -eq 0 ] && [ $error_memory -ne 0 ]; then
    exit $ERROR_MEMORY
elif [ $error_answer -ne 0 ] && [ $error_memory -ne 0 ]; then
    exit $ERROR_MEMORY_ANSWER
fi
