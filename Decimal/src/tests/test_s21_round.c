#include "../s21_decimal.h"
#include "test.h"

START_TEST(test_s21_round_basic) {
  s21_decimal value, result;

  // Test 1: Positive number with decimal part > 0.5
  value = (s21_decimal){{12345, 0, 0, 0x00020000}};  // 123.45
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Positive number with decimal part < 0.5
  value = (s21_decimal){{12344, 0, 0, 0x00020000}};  // 123.44
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: Positive number with decimal part exactly 0.5
  value = (s21_decimal){{12345, 0, 0, 0x00020000}};  // 123.45 (rounds to 123)
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
}
END_TEST

START_TEST(test_s21_round_negative) {
  s21_decimal value, result;

  // Test 1: Negative number with decimal part > 0.5
  value = (s21_decimal){{1236, 0, 0, 0x80010000}};  // -123.6
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 124);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 2: Negative number with decimal part < 0.5
  value = (s21_decimal){{12344, 0, 0, 0x80020000}};  // -123.44
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Negative number with decimal part exactly 0.5
  value = (s21_decimal){{123456789, 0, 0, 0x80030000}};  // -123456.789
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123457);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_round_edge_cases) {
  s21_decimal value, result;

  // Test 1: Zero with scale
  value = (s21_decimal){{0, 0, 0, 0x00050000}};  // 0.00000
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Negative zero
  value = (s21_decimal){{0, 0, 0, 0x80050000}};  // -0.00000
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);  // Should preserve negative sign

  // Test 3: Already rounded number (scale 0)
  value = (s21_decimal){{123, 0, 0, 0}};  // 123
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_round_precision) {
  s21_decimal result;

  // Test different scale values
  int scales[] = {1, 2, 5, 10, 15, 20, 28};
  for (unsigned i = 0; i < sizeof(scales) / sizeof(scales[0]); i++) {
    int scale = scales[i];
    s21_decimal value = {
        {123456789, 0, 0, scale << 16}};  // 123456789 with scale

    s21_round(value, &result);

    // After rounding, scale should be 0
    ck_assert_int_eq(get_exp(result), 0);
    ck_assert_int_eq(get_sign(result), 0);
  }
}
END_TEST

START_TEST(test_s21_round_half_cases) {
  s21_decimal value, result;

  // Test cases where remainder is exactly 5 (half)
  value = (s21_decimal){{5, 0, 0, 0x00010000}};  // 0.5
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 1);  // Should round up to 1

  value = (s21_decimal){{15, 0, 0, 0x00010000}};  // 1.5
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 2);  // Should round up to 2

  value = (s21_decimal){{25, 0, 0, 0x00010000}};  // 2.5
  s21_round(value, &result);
  ck_assert_int_eq(result.bits[0], 3);  // Should round up to 3
}
END_TEST

START_TEST(test_s21_round_boundary_values) {
  s21_decimal result;

  // Test boundary values for rounding
  struct TestCase {
    int value;
    int scale;
    int expected;
  } test_cases[] = {
      {499, 2, 5},    // 4.99 -> 5
      {500, 2, 5},    // 5.00 -> 5
      {501, 2, 5},    // 5.01 -> 5
      {1499, 2, 15},  // 14.99 -> 15
      {1500, 2, 15},  // 15.00 -> 15
      {1501, 2, 15},  // 15.01 -> 15
  };

  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    s21_decimal value = {
        {test_cases[i].value, 0, 0, test_cases[i].scale << 16}};
    s21_round(value, &result);
    ck_assert_int_eq(result.bits[0], test_cases[i].expected);
    ck_assert_int_eq(get_exp(result), 0);
  }
}
END_TEST

TCase *s21_round_tcase(void) {
  TCase *tc = tcase_create("s21_round");

  tcase_add_test(tc, test_s21_round_basic);
  tcase_add_test(tc, test_s21_round_negative);
  tcase_add_test(tc, test_s21_round_edge_cases);
  tcase_add_test(tc, test_s21_round_precision);
  tcase_add_test(tc, test_s21_round_half_cases);
  tcase_add_test(tc, test_s21_round_boundary_values);

  return tc;
}