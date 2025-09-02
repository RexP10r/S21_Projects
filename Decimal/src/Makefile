CC=gcc
CFLAGS=-Wall -Wextra -Werror -lm -std=c11 #-fsanitize=address,undefined
TEST_FLAGS=-lcheck -lm -lpthread -lsubunit
GCOV_FLAGS=-fprofile-arcs -ftest-coverage

SRC_DIR=.
TEST_DIR=tests
S_TEST_DIR=simple_tests
BUILD_DIR=.
REPORT_DIR=report
FUNCS_DIR = funcs/

SOURCES=$(shell find $(SRC_DIR) -name "*.c" -not -path "./$(TEST_DIR)/*")
OBJECTS=$(SOURCES:.c=.o)
GCOV_OBJECTS=$(SOURCES:.c=.gcov.o)

TEST_SOURCES=$(shell find $(TEST_DIR) -name "*.c")
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)

.PHONY: all rebuild clean 

all: s21_decimal.a

s21_decimal.a: $(OBJECTS)
	ar rcs s21_decimal.a $(OBJECTS)
	ranlib s21_decimal.a

s21_decimal_gcov.a: $(GCOV_OBJECTS)
	ar rcs s21_decimal_gcov.a $(GCOV_OBJECTS)
	ranlib s21_decimal_gcov.a

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.gcov.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

test: $(TEST_OBJECTS) s21_decimal_gcov.a
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_OBJECTS) s21_decimal_gcov.a $(TEST_FLAGS) -o test
	./test

gcov_report:
	mkdir -p $(REPORT_DIR)
	lcov -t "s21_decimal_test" -o $(REPORT_DIR)/s21_decimal_test.info -c -d .
	genhtml -o $(REPORT_DIR) $(REPORT_DIR)/s21_decimal_test.info
	
clean:
	rm -rf *.o
	rm -rf *.deb
	rm -rf s21_decimal.a
	rm -rf s21_decimal_gcov.a
	rm -rf test
	rm -rf simple_test
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf $(S_TEST_DIR)/*.o
	rm -rf $(TEST_DIR)/*.o
	rm -rf $(TEST_DIR)/*.gcda
	rm -rf $(TEST_DIR)/*.gcno
	rm -rf $(OBJECTS) $(FUNCS_DIR)*.o
	rm -rf $(OBJECTS) $(FUNCS_DIR)*.gcno
	rm -rf $(OBJECTS) $(FUNCS_DIR)*.gcda

clean_report:
	rm -rf $(REPORT_DIR)

format:
	chmod +x *
	./format.sh

rebuild: clean format all

retest: rebuild test

retest_simple: rebuild simple_test
