CC=gcc -Werror -Wall -Wextra -std=c11 
TARGET1=s21_cat

all: s21_cat

s21_cat: filef.o main.o s21_cat.o
	$(CC) main.o filef.o s21_cat.o -o $(TARGET1) # -fsanitize=leak

main.o: 
	$(CC) -c main.c
filef.o: 
	$(CC) -c filef.c
s21_cat.o: 
	$(CC) -c s21_cat.c	

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
