#! /bin/bash

flags=("-s" "-t" "-e" "-n" "-b" "-ts" "-b -e" "-btse" "-r")
files="test_5_cat.txt null.txt filef.h null"

out_file=out.txt

test() {
    local flag="$1"
    local result=""
    ./s21_cat $flag $files > mycat.txt 2> /dev/null
    cat $flag $files > bashcat.txt 2> /dev/null

    if diff -q "mycat.txt" "bashcat.txt" > /dev/null; then
        result="\033[42mSUCCESS"
    else
        result="\033[41mFAIL"
    fi

    echo -e "./s21_cat|$flag|$files|=>|$result\033[0m" >> $out_file
}

for ((i=0;i<9;i++));do
    test "${flags[$i]}"
done

column -t -s "|" "$out_file"

rm -f mycat.txt bashcat.txt out.txt

# valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all