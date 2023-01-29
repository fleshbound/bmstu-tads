#!/bin/bash

EXIT_SUCCESS=0
ERROR_ANSWER=1
ERROR_MEMORY=2
EXIT_SUCCESS_VALGRIND=3
ERROR_MEMORY_ANSWER=4

ERROR_NONE=255

file_stream_in="$1"
file_stream_out_except="$2"
file_app_args="$3"
file_stream_out_current="tmp.txt"

#cd ../../

if [ -z "$file_stream_out_except" ]; then
    echo "No tests were found, quitting..."
    exit $ERROR_NONE
fi

declare -a app_args=()
if [ ! \( -z "$file_app_args"  \) ]; then
    argstr=$( grep -v "^$" "$file_app_args" )
    app_args=($argstr)
    
#    echo "ARGUMENTS:"
#    echo "${app_args[@]}"
fi

touch $file_stream_out_current
error_memory=0
if [ "$FLAG_VAL" = "1" ]; then
    {
        valgrind --tool=memcheck --log-file=log.txt --quiet ./app.exe "${app_args[@]}" < "$file_stream_in"
        res_code=$?
    } > $file_stream_out_current

#    echo "FILE NAME IN:"
#    echo $file_stream_in
    
#    echo "EXPECTED:"
#    cat $file_stream_out_except

#    echo "GOT:"
#    cat $file_stream_out_current
    
    if [ -s "log.txt" ]; then
        error_memory=1
    fi
else
    ./app.exe "${app_args[@]}" <"$file_stream_in" >"$file_stream_out_current"
    res_code=$?

#    echo "FILE NAME IN:"
#    echo $file_stream_in

#     echo "EXPECTED:"
#     cat $file_stream_out_except

#     echo "GOT:"
#     cat $file_stream_out_current
    
#    echo "RESULT CODE:"
#    echo "$res_code"
fi

error_answer=0
if [ $res_code -ne 0 ]; then
    echo "RESULT CODE: $res_code"
    
    error_answer=1
else
    ./func_tests/scripts/comparator.sh "$file_stream_out_current" "$file_stream_out_except"
    result=$?
    
    if [ $result -ne $EXIT_SUCCESS ]; then
        #echo "EXPECTED:"
        #cat $file_stream_out_except

        #echo "GOT:"
        #cat $file_stream_out_current

        error_answer=1
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
