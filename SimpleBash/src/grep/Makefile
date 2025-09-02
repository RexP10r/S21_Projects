CC=gcc -Werror -Wall -Wextra -std=c11
TARGET1=s21_grep

all: s21_grep

s21_grep: filef.o main.o s21_grep.o
	$(CC) main.o filef.o s21_grep.o -o $(TARGET1) # -fsanitize=leak

main.o: 
	$(CC) -c main.c
filef.o: 
	$(CC) -c filef.c
s21_grep.o: 
	$(CC) -c s21_grep.c	

clean:
	rm -f *.o

rebuild: format all clean
test: 
	chmod 777 test.sh
	./test.sh
retest: rebuild test

format:
	clang-format -style=Google -i *.c *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
