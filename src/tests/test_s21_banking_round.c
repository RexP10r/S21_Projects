#include "../s21_decimal.h"
#include "test.h"

START_TEST(test_s21_banking_round_basic) {
  s21_decimal value, result;

  // Test 1: Positive number, decimal part > 0.5 (round up)
  value = (s21_decimal){{12346, 0, 0, 0x00020000}};  // 123.46
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Positive number, decimal part < 0.5 (round down)
  value = (s21_decimal){{12344, 0, 0, 0x00020000}};  // 123.44
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: Positive integer (no change)
  value = (s21_decimal){{123, 0, 0, 0}};  // 123
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_banking_round_negative) {
  s21_decimal value, result;

  // Test 1: Negative number, decimal part > 0.5 (round up)
  value = (s21_decimal){{12346, 0, 0, 0x80020000}};  // -123.46
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 2: Negative number, decimal part < 0.5 (round down)
  value = (s21_decimal){{12344, 0, 0, 0x80020000}};  // -123.44
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Negative integer (no change)
  value = (s21_decimal){{123, 0, 0, 0x80000000}};  // -123
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_banking_round_half_cases) {
  s21_decimal value, result;

  // Test 1: 0.5 with even integer part (round down)
  value = (s21_decimal){{5, 0, 0, 0x00010000}};  // 0.5 -> 0 (even)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 2: 1.5 with odd integer part (round up)
  value = (s21_decimal){{15, 0, 0, 0x00010000}};  // 1.5 -> 2 (odd)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: 2.5 with even integer part (round down)
  value = (s21_decimal){{25, 0, 0, 0x00010000}};  // 2.5 -> 2 (even)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 4: 3.5 with odd integer part (round up)
  value = (s21_decimal){{35, 0, 0, 0x00010000}};  // 3.5 -> 4 (odd)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(get_exp(result), 0);
}
END_TEST

START_TEST(test_s21_banking_round_negative_half_cases) {
  s21_decimal value, result;

  // Test 1: -0.5 with even integer part (round down)
  value = (s21_decimal){{5, 0, 0, 0x80010000}};  // -0.5 -> 0 (even)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 2: -1.5 with odd integer part (round up)
  value = (s21_decimal){{15, 0, 0, 0x80010000}};  // -1.5 -> -2 (odd)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: -2.5 with even integer part (round down)
  value = (s21_decimal){{25, 0, 0, 0x80010000}};  // -2.5 -> -2 (even)
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_banking_round_edge_cases) {
  s21_decimal value, result;

  // Test 1: Zero with scale
  value = (s21_decimal){{0, 0, 0, 0x00050000}};  // 0.00000
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 0);

  // Test 2: Negative zero
  value = (s21_decimal){{0, 0, 0, 0x80050000}};  // -0.00000
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  // Test 3: Very small decimal part (positive)
  value =
      (s21_decimal){{1, 0, 0, 0x001C0000}};  // 0.0000000000000000000000000001
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 4: Very small decimal part (negative)
  value =
      (s21_decimal){{1, 0, 0, 0x801C0000}};  // -0.0000000000000000000000000001
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_banking_round_large_numbers) {
  s21_decimal value, result;

  // Test 1: Large positive number with rounding up
  value = (s21_decimal){{999999996, 0, 0, 0x00010000}};  // 99999999.6
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 100000000);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 2: Large positive number with rounding down
  value = (s21_decimal){{999999994, 0, 0, 0x00010000}};  // 99999999.4
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 99999999);
  ck_assert_int_eq(get_exp(result), 0);

  // Test 3: Large negative number with rounding
  value = (s21_decimal){{999999996, 0, 0, 0x80010000}};  // -99999999.6
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 100000000);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_banking_round_different_scales) {
  s21_decimal result;

  // Test different scale values
  int scales[] = {1, 2, 5, 10, 15, 20, 28};
  for (unsigned i = 0; i < sizeof(scales) / sizeof(scales[0]); i++) {
    int scale = scales[i];
    s21_decimal value = {{123456789, 0, 0, scale << 16}};

    s21_banking_round(value, &result);

    ck_assert_int_eq(get_exp(result), 0);
    ck_assert_int_eq(get_sign(result), 0);
  }
}
END_TEST

START_TEST(test_s21_banking_round_fractional_only) {
  s21_decimal value, result;

  // Test positive fractional numbers
  value = (s21_decimal){{45, 0, 0, 0x00020000}};  // 0.45 -> 0
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);

  value = (s21_decimal){{55, 0, 0, 0x00020000}};  // 0.55 -> 1
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_exp(result), 0);

  // Test negative fractional numbers
  value = (s21_decimal){{45, 0, 0, 0x80020000}};  // -0.45 -> 0
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);

  value = (s21_decimal){{55, 0, 0, 0x80020000}};  // -0.55 -> -1
  s21_banking_round(value, &result);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_exp(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_s21_banking_round_boundary_values) {
  s21_decimal result;

  // Test boundary values for banking round
  int test_cases[] = {
      499,   // 4.99 -> 5
      500,   // 5.00 -> 5 (even, round down)
      501,   // 5.01 -> 5
      1499,  // 14.99 -> 15
      1500,  // 15.00 -> 15 (odd, round up)
      1501,  // 15.01 -> 15
      2499,  // 24.99 -> 25
      2500,  // 25.00 -> 25 (even, round down)
      2501,  // 25.01 -> 25
  };
  int expected[] = {5, 5, 5, 15, 15, 15, 25, 25, 25};

  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    s21_decimal value = {{test_cases[i], 0, 0, 2 << 16}};
    s21_banking_round(value, &result);
    ck_assert_int_eq(result.bits[0], expected[i]);
    ck_assert(!get_exp(result));
    ck_assert(!get_sign(result));
  }
}
END_TEST

START_TEST(test_s21_banking_round_exact_half_cases) {
  s21_decimal value, result;

  // Test exact half cases with different integer parts
  struct TestCase {
    int integer_part;
    int expected;
  } test_cases[] = {
      {0, 0},   // 0.5 -> 0 (even)
      {1, 2},   // 1.5 -> 2 (odd)
      {2, 2},   // 2.5 -> 2 (even)
      {3, 4},   // 3.5 -> 4 (odd)
      {4, 4},   // 4.5 -> 4 (even)
      {5, 6},   // 5.5 -> 6 (odd)
      {6, 6},   // 6.5 -> 6 (even)
      {7, 8},   // 7.5 -> 8 (odd)
      {8, 8},   // 8.5 -> 8 (even)
      {9, 10},  // 9.5 -> 10 (odd)
  };

  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    value =
        (s21_decimal){{test_cases[i].integer_part * 10 + 5, 0, 0, 0x00010000}};
    s21_banking_round(value, &result);
    ck_assert_int_eq(result.bits[0], test_cases[i].expected);
    ck_assert_int_eq(get_exp(result), 0);
  }

  // Test negative exact half cases
  for (unsigned i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
    value =
        (s21_decimal){{test_cases[i].integer_part * 10 + 5, 0, 0, 0x80010000}};
    s21_banking_round(value, &result);
    ck_assert_int_eq(result.bits[0], test_cases[i].expected);
    ck_assert_int_eq(get_exp(result), 0);
    ck_assert_int_eq(get_sign(result), 1);
  }
}
END_TEST

TCase *s21_banking_round_tcase(void) {
  TCase *tc = tcase_create("s21_banking_round");

  tcase_add_test(tc, test_s21_banking_round_basic);
  tcase_add_test(tc, test_s21_banking_round_negative);
  tcase_add_test(tc, test_s21_banking_round_half_cases);
  tcase_add_test(tc, test_s21_banking_round_negative_half_cases);
  tcase_add_test(tc, test_s21_banking_round_edge_cases);
  tcase_add_test(tc, test_s21_banking_round_large_numbers);
  tcase_add_test(tc, test_s21_banking_round_different_scales);
  tcase_add_test(tc, test_s21_banking_round_fractional_only);
  tcase_add_test(tc, test_s21_banking_round_boundary_values);
  tcase_add_test(tc, test_s21_banking_round_exact_half_cases);

  return tc;
}