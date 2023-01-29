#!/bin/bash

# --FLAGS--
export FLAG_VAL=0

# EXIT CODES
EXIT_SUCCESS=0
ERROR_ANSWER=1
ERROR_MEMORY=2
EXIT_SUCCESS_VALGRIND=3
ERROR_MEMORY_ANSWER=4

# Colors
RED='\033[0;31m'
NC='\033[0m' # No Color
LGreen='\033[1;32m'

# Updates flags (ignores incorrect)
function update_flags
{
    if [ "$1" = "-USE_VALGRIND" ]; then
        FLAG_VAL=1
    fi
}

# Checks positive tests and returns number of failures
function check_positive
{
    echo -e "\n---POSITIVE-TESTS---\n"

    files_pos_in=$( find ./func_tests/data -regex "\./func_tests/data/pos_[0-9]+_in.txt" | sort -t '\0' -n )
    i=1
    q_pos=0
    num=""
    
    for file_stream_in in $files_pos_in; do
        # '0' -> '00', '10' -> '10' (correct number for test file name)
        if [ $i -lt 10 ]; then
            num="0$i"
        else
            num="$i"
        fi
        
        file_stream_out=$( find ./func_tests/data -regextype posix-egrep -regex "\./func_tests/data/pos_""$num""_out.txt" )
        file_app_args=$( find ./func_tests/data -regextype posix-egrep -regex "\./func_tests/data/pos_""$num""_args.txt" )

        # Compairing ethalon and current output
        ./func_tests/scripts/pos_case.sh "$file_stream_in" "$file_stream_out" "$file_app_args"
        result=$?
	
        # Single result output
        q_pos=$(( q_pos + 1 ))
        if [ $result -eq $EXIT_SUCCESS ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${NC}NO VALGRIND${NC}"
            q_pos=$(( q_pos - 1 ))
        elif [ $result -eq $EXIT_SUCCESS_VALGRIND ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${LGreen}MEMORY OK${NC}"
            q_pos=$(( q_pos - 1 ))
        elif [ $result -eq $ERROR_ANSWER ]; then
            if [ "$FLAG_VAL" = "1" ]; then
                echo -e "TEST $num: ${RED}ANSWER ERROR${NC}; ${LGreen}MEMORY OK${NC}"
            else
                echo -e "TEST $num: ${RED}ANSWER ERROR${NC}; ${NC}NO VALGRIND${NC}"
            fi
        elif [ $result -eq $ERROR_MEMORY ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${RED}MEMORY ERROR${NC}"
        elif [ $result -eq $ERROR_MEMORY_ANSWER ]; then
            echo -e "TEST $num: ${LGreen}ANSWER ERROR${NC}; ${RED}MEMORY ERROR${NC}"
        fi

        i=$(( i + 1 ))
    done
    
    # Total output
    echo -e "Failed $q_pos POSITIVE test(s) of total $(( i - 1 ))"

    return $q_pos
}

# Checks negative tests and returns number of failures
function check_negative
{
    echo -e "\n---NEGATIVE-TESTS---\n"

    files_neg_in=$( find ./func_tests/data -regex "\./func_tests/data/neg_[0-9]*_in.txt" | sort -t '\0' -n )
    i=1
    q_neg=0
    num=""

    for file_stream_in in $files_neg_in; do
        # '0' -> '00', '10' -> '10' (correct number for test file name)
        if [ $i -lt 10 ]; then
            num="0$i"
        else
            num="$i"
        fi

        file_stream_out_except=$( find ./func_tests/data -regextype posix-egrep -regex "\./func_tests/data/neg_""$num""_out.txt" )
        file_app_args=$( find ./func_tests/data -regextype posix-egrep -regex "\./func_tests/data/neg_""$num""_args.txt" )

        # Compairing ethalon and current output (by exit code or not)
        ./func_tests/scripts/neg_case.sh "$file_stream_in" "$file_stream_out_except" "$file_app_args"
        result=$?

        # Single result output
        q_neg=$(( q_neg + 1 ))
        if [ $result -eq $EXIT_SUCCESS ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${NC}NO VALGRIND${NC}"
            q_neg=$(( q_neg - 1 ))
        elif [ $result -eq $EXIT_SUCCESS_VALGRIND ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${LGreen}MEMORY OK${NC}"
            q_neg=$(( q_neg - 1 ))
        elif [ $result -eq $ERROR_ANSWER ]; then
            if [ "$FLAG_VAL" = "1" ]; then
                echo -e "TEST $num: ${RED}ANSWER ERROR${NC}; ${LGreen}MEMORY OK${NC}"
            else
                echo -e "TEST $num: ${RED}ANSWER ERROR${NC}; ${NC}NO VALGRIND${NC}"
            fi
        elif [ $result -eq $ERROR_MEMORY ]; then
            echo -e "TEST $num: ${LGreen}ANSWER OK${NC}; ${RED}MEMORY ERROR${NC}"
        elif [ $result -eq $ERROR_MEMORY_ANSWER ]; then
            echo -e "TEST $num: ${RED}ANSWER ERROR${NC}; ${RED}MEMORY ERROR${NC}"
        fi
        
        i=$(( i + 1 ))
    done

    # Total output
    echo -e "Failed $q_neg NEGATIVE test(s) of total $(( i - 1 ))"

    return $q_neg
}

# ---------------
# ---MAIN-CODE---
# ---v-v-v-v-v---

cd ../../

if [ ! \( -f ./app.exe \) ]; then
    echo "ERROR: No executable file(s) was(were) found"
    echo "NOTE: Try running a build script in one of parental directories"
    echo "Quitting..."
    exit 100
fi

update_flags "$1"

check_positive
pos=$?

check_negative
neg=$?

echo -e "\nTotal: $(( pos + neg )) test(s) was(were) failed"

exit $(( pos + neg ))
