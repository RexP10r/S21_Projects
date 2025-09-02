#include "../s21_decimal.h"
#include "test.h"

/*int are_decimals_equal(s21_decimal a, s21_decimal b) {
  return a.bits[0] == b.bits[0] && a.bits[1] == b.bits[1] &&
         a.bits[2] == b.bits[2] && a.bits[3] == b.bits[3];
}*/

unsigned int safe_abs(int x) {
  if (x == -2147483648) return 2147483648u;
  return (x < 0) ? -x : x;
}

START_TEST(test_s21_from_int_to_decimal_basic) {
  s21_decimal result;
  int return_code;

  return_code = s21_from_int_to_decimal(123, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(-456, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(0, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(2147483647, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 2147483647);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(-2147483648, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_uint_eq(result.bits[0], 2147483648u);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_edge_cases) {
  s21_decimal result;
  int return_code;

  return_code = s21_from_int_to_decimal(1, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(-1, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(1000000, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 1000000);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);

  return_code = s21_from_int_to_decimal(-999999, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 999999);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_null_pointer) {
  int return_code = s21_from_int_to_decimal(123, NULL);
  ck_assert_int_eq(return_code, 1);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_multiple_calls) {
  s21_decimal result1, result2, result3;

  s21_from_int_to_decimal(10, &result1);
  s21_from_int_to_decimal(-20, &result2);
  s21_from_int_to_decimal(30, &result3);

  ck_assert_int_eq(result1.bits[0], 10);
  ck_assert_int_eq(result1.bits[3] & (1u << 31), 0);

  ck_assert_int_eq(result2.bits[0], 20);
  ck_assert_int_ne(result2.bits[3] & (1u << 31), 0);

  ck_assert_int_eq(result3.bits[0], 30);
  ck_assert_int_eq(result3.bits[3] & (1u << 31), 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_boundary_values) {
  s21_decimal result;

  const int test_values[] = {1,    -1,   2,     -2,         100,
                             -100, 1000, -1000, 2147483647, -2147483648,
                             0,    999,  -999};

  for (int i = 0; i < 13; i++) {
    int value = test_values[i];
    int return_code = s21_from_int_to_decimal(value, &result);

    ck_assert_int_eq(return_code, 0);
    ck_assert_int_eq(result.bits[0], safe_abs(value));
    ck_assert_int_eq(result.bits[1], 0);
    ck_assert_int_eq(result.bits[2], 0);

    if (value < 0) {
      ck_assert_int_ne(result.bits[3] & (1u << 31), 0);
    } else {
      ck_assert_int_eq(result.bits[3] & (1u << 31), 0);
    }
  }
}
END_TEST

TCase *s21_from_int_to_decimal_tcase(void) {
  TCase *tc = tcase_create("s21_from_int_to_decimal");

  tcase_add_test(tc, test_s21_from_int_to_decimal_basic);
  tcase_add_test(tc, test_s21_from_int_to_decimal_edge_cases);
  tcase_add_test(tc, test_s21_from_int_to_decimal_null_pointer);
  tcase_add_test(tc, test_s21_from_int_to_decimal_multiple_calls);
  tcase_add_test(tc, test_s21_from_int_to_decimal_boundary_values);

  return tc;
}