#include "../s21_decimal.h"
#include "test.h"

START_TEST(test_s21_truncate_basic) {
  s21_decimal value, result;

  // Test 1: Positive number with decimal part
  value = (s21_decimal){{12345, 0, 0, 0x00020000}};  // 123.45
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Positive number with larger decimal part
  value = (s21_decimal){{12399, 0, 0, 0x00020000}};  // 123.99
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: Positive number with scale 1
  value = (s21_decimal){{1234, 0, 0, 0x00010000}};  // 123.4
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
}
END_TEST

START_TEST(test_s21_truncate_negative) {
  s21_decimal value, result;

  // Test 1: Negative number with decimal part
  value = (s21_decimal){{12345, 0, 0, 0x80020000}};  // -123.45
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 2: Negative number with larger decimal part
  value = (s21_decimal){{12399, 0, 0, 0x80020000}};  // -123.99
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_truncate_edge_cases) {
  s21_decimal value, result;

  // Test 1: Zero with scale
  value = (s21_decimal){{0, 0, 0, 0x00050000}};  // 0.00000
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Negative zero
  value = (s21_decimal){{0, 0, 0, 0x80050000}};  // -0.00000
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Already truncated number (scale 0)
  value = (s21_decimal){{123, 0, 0, 0}};  // 123
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 4: Very small decimal part
  value =
      (s21_decimal){{1, 0, 0, 0x001C0000}};  // 0.0000000000000000000000000001
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_large_numbers) {
  s21_decimal value, result;

  // Test 1: Large number with decimal part
  value = (s21_decimal){{999999999, 0, 0, 0x00010000}};  // 99999999.9
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 99999999);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 2: Multi-word decimal
  value = (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}};
  int ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_truncate_fractional_only) {
  s21_decimal value, result;

  // Test numbers that are purely fractional
  value = (s21_decimal){{45, 0, 0, 0x00020000}};  // 0.45
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  value = (s21_decimal){{999, 0, 0, 0x00030000}};  // 0.999
  s21_truncate(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_truncate_null_result) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};  // 123.45
  int ret = s21_truncate(value, NULL);
  ck_assert_int_eq(ret, 1);

  s21_decimal result;
  ret = s21_truncate(value, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 123);
}
END_TEST

START_TEST(test_s21_truncate_boundary_values) {
  s21_decimal result;

  // Test boundary values for truncation
  struct TestCase {
    int value;
    int scale;
    int expected;
  } test_cases[] = {
      {499, 2, 4},   // 4.99 -> 4
      {500, 2, 5},   // 5.00 -> 5
      {501, 2, 5},   // 5.01 -> 5
      {999, 3, 0},   // 0.999 -> 0
      {1000, 3, 1},  // 1.000 -> 1
      {1999, 3, 1},  // 1.999 -> 1
  };

  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    s21_decimal value = {
        {test_cases[i].value, 0, 0, test_cases[i].scale << 16}};
    s21_truncate(value, &result);
    ck_assert_int_eq(result.bits[0], test_cases[i].expected);
    ck_assert_int_eq(get_exp(result), 0);
  }
}
END_TEST

TCase *s21_truncate_tcase(void) {
  TCase *tc = tcase_create("s21_truncate");

  tcase_add_test(tc, test_s21_truncate_basic);
  tcase_add_test(tc, test_s21_truncate_negative);
  tcase_add_test(tc, test_s21_truncate_edge_cases);
  tcase_add_test(tc, test_s21_truncate_large_numbers);
  tcase_add_test(tc, test_s21_truncate_fractional_only);
  tcase_add_test(tc, test_s21_truncate_null_result);
  tcase_add_test(tc, test_s21_truncate_boundary_values);

  return tc;
}