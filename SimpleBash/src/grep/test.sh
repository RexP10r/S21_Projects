#! /bin/bash

file1="main.c"
file2="filef.c"
file3="filef.h"
file4="Makefile"
file5="test.sh"
patts="patterns.txt"

flags=("-i" "-e" "-v" "-c" "-l" "-n" "-d" "-ohn" "-f" "-ilo -e" "-e da" "a" "$file2 null")
patterns=("a" "[0-9]" "[0-9]" "ag" "!" "f" "da" "3.14" "$patts" "zda" "-f $patts -no" "-sn" "-ino -e da -e o")
files=(
"$file2" 
"$file1" 
"$file3" 
"null.txt null" 
"$file3" 
"$file3 $file4" 
"$file2 $file3" 
"$file2 $file4" 
"$file1 $patts" 
"$file3 $file2 $file1"
"$file2"
"$file2 null $file5" 
"$file1 null.txt"
)

out_file="out.txt"

test() {
    local flag="$1"
    local pattern="$2"
    local file="$3"
    local result=""
    ./s21_grep $flag $pattern $file > mygrep.txt 2> /dev/null
    grep $flag $pattern $file > bashgrep.txt 2> /dev/null

    if diff -q "mygrep.txt" "bashgrep.txt" > /dev/null; then
        result="\033[42mSUCCESS"
    else
        result="\033[41mFAIL"
    fi

    echo -e "./s21_grep|$flag|$pattern|$file|=>|$result\033[0m" >> $out_file
}

for ((i=0;i<13;i++));do
    test "${flags[$i]}" "${patterns[$i]}" "${files[$i]}"
done

column -t -s "|" "$out_file"

rm -f mygrep.txt bashgrep.txt out.txt

# valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all