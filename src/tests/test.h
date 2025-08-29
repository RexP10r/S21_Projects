#ifndef TESTS
#define TESTS

#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "../s21_decimal.h"

TCase *s21_is_equal_tcase(void);
TCase *s21_is_not_equal_tcase(void);

TCase *s21_is_greater_tcase(void);
TCase *s21_is_greater_or_equal_tcase(void);

TCase *s21_is_less_tcase(void);
TCase *s21_is_less_or_equal_tcase(void);

TCase *s21_div_tcase(void);
TCase *s21_mul_tcase(void);
TCase *s21_add_tcase(void);
TCase *s21_sub_tcase(void);

TCase *s21_negate_tcase(void);
TCase *s21_truncate_tcase(void);
TCase *s21_floor_tcase(void);
TCase *s21_round_tcase(void);
TCase *s21_banking_round_tcase(void);

TCase *s21_from_float_to_decimal_tcase(void);
TCase *s21_from_decimal_to_float_tcase(void);

TCase *s21_from_int_to_decimal_tcase(void);
TCase *s21_from_decimal_to_int_tcase(void);

TCase *s21_negate_tcase(void);
TCase *s21_truncate_tcase(void);
TCase *s21_floor_tcase(void);
TCase *s21_round_tcase(void);
TCase *s21_banking_round_tcase(void);
TCase *s21_normalization_tcase(void);

#endif