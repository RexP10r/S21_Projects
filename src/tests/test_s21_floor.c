#include "../s21_decimal.h"
#include "test.h"

START_TEST(test_s21_floor_basic) {
  s21_decimal value, result;

  // Test 1: Positive number with decimal part
  value = (s21_decimal){{12345, 0, 0, 0x00020000}};  // 123.45
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Positive number with larger decimal part
  value = (s21_decimal){{12399, 0, 0, 0x00020000}};  // 123.99
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: Positive integer (no change)
  value = (s21_decimal){{123, 0, 0, 0}};  // 123
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_floor_negative) {
  s21_decimal value, result;

  // Test 1: Negative number with decimal part - should floor down
  value = (s21_decimal){{12345, 0, 0, 0x80020000}};  // -123.45
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 124);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 2: Negative number with larger decimal part
  value = (s21_decimal){{12399, 0, 0, 0x80020000}};  // -123.99
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 124);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Negative integer (no change)
  value = (s21_decimal){{123, 0, 0, 0x80000000}};  // -123
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_floor_edge_cases) {
  s21_decimal value, result;

  // Test 1: Zero with scale
  value = (s21_decimal){{0, 0, 0, 0x00050000}};  // 0.00000
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Negative zero
  value = (s21_decimal){{0, 0, 0, 0x80050000}};  // -0.00000
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Very small decimal part (positive)
  value =
      (s21_decimal){{1, 0, 0, 0x001C0000}};  // 0.0000000000000000000000000001
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 4: Very small decimal part (negative)
  value =
      (s21_decimal){{1, 0, 0, 0x801C0000}};  // -0.0000000000000000000000000001
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_floor_large_numbers) {
  s21_decimal value, result;

  // Test 1: Large positive number with decimal part
  value = (s21_decimal){{999999999, 0, 0, 0x00010000}};  // 99999999.9
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 99999999);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 2: Large negative number with decimal part
  value = (s21_decimal){{999999999, 0, 0, 0x80010000}};  // -99999999.9
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 100000000);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_floor_fractional_only) {
  s21_decimal value, result;

  // Test positive fractional numbers
  value = (s21_decimal){{45, 0, 0, 0x00020000}};  // 0.45
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  value = (s21_decimal){{999, 0, 0, 0x00030000}};  // 0.999
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  // Test negative fractional numbers
  value = (s21_decimal){{45, 0, 0, 0x80020000}};  // -0.45
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  value = (s21_decimal){{999, 0, 0, 0x80030000}};  // -0.999
  s21_floor(value, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_floor_null_result) {
  s21_decimal value = {{12345, 0, 0, 0x00020000}};  // 123.45

  // Test with NULL result pointer
  int ret = s21_floor(value, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_floor_exact_integers) {
  s21_decimal result;

  // Test numbers that are already integers with different scales
  struct TestCase {
    int value;
    int scale;
    int sign;
  } test_cases[] = {
      {100, 2, 0},    // 1.00 -> 1
      {100, 2, 1},    // -1.00 -> -1
      {5000, 3, 0},   // 5.000 -> 5
      {5000, 3, 1},   // -5.000 -> -5
      {12300, 2, 0},  // 123.00 -> 123
      {12300, 2, 1},  // -123.00 -> -123
  };

  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    int sign = test_cases[i].sign ? 0x80000000 : 0;
    s21_decimal value = {
        {test_cases[i].value, 0, 0, (test_cases[i].scale << 16) | sign}};
    s21_floor(value, &result);
    ck_assert_int_eq(result.bits[0],
                     test_cases[i].value / (int)pow(10, test_cases[i].scale));
    ck_assert_int_eq(get_exp(result), 0);
    ck_assert_int_eq(get_sign(result), test_cases[i].sign);
  }
}
END_TEST

TCase *s21_floor_tcase(void) {
  TCase *tc = tcase_create("s21_floor");

  tcase_add_test(tc, test_s21_floor_basic);
  tcase_add_test(tc, test_s21_floor_negative);
  tcase_add_test(tc, test_s21_floor_edge_cases);
  tcase_add_test(tc, test_s21_floor_large_numbers);
  tcase_add_test(tc, test_s21_floor_fractional_only);
  tcase_add_test(tc, test_s21_floor_null_result);
  tcase_add_test(tc, test_s21_floor_exact_integers);

  return tc;
}