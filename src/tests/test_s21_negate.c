#include "../s21_decimal.h"
#include "test.h"

static s21_decimal create_decimal(int value, int scale) {
  s21_decimal dec;
  dec.bits[0] = (value < 0) ? -value : value;
  dec.bits[1] = 0;
  dec.bits[2] = 0;
  dec.bits[3] = (value < 0) ? (1u << 31) : 0;
  if (scale > 0) {
    dec.bits[3] |= (scale << 16);
  }
  return dec;
}

START_TEST(test_s21_negate_basic) {
  s21_decimal result;
  int return_code;

  // Positive to Negative
  s21_decimal a = create_decimal(5, 0);
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);
  ck_assert_int_eq(result.bits[3] & 0x7FFFFFFF, a.bits[3] & 0x7FFFFFFF);

  // Negative to Positive
  a = create_decimal(-5, 0);
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);
  ck_assert_int_eq(result.bits[3] & 0x7FFFFFFF, a.bits[3] & 0x7FFFFFFF);
}
END_TEST

START_TEST(test_s21_negate_with_scale) {
  s21_decimal result;
  int return_code;

  // Positive with scale to Negative
  s21_decimal a = create_decimal(123, 2);  // 1.23
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 2);

  // Negative with scale to Positive
  a = create_decimal(-456, 1);  // -45.6
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 456);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 1);
}
END_TEST

START_TEST(test_s21_negate_null_pointer) {
  s21_decimal a = create_decimal(5, 0);

  int return_code = s21_negate(a, NULL);
  ck_assert_int_eq(return_code, 1);
}
END_TEST

START_TEST(test_s21_negate_large_numbers) {
  s21_decimal result;
  int return_code;

  // Large positive number
  s21_decimal a = create_decimal(1000000, 0);
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 1000000);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);

  // Large negative number
  a = create_decimal(-999999, 0);
  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 999999);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);
}
END_TEST

START_TEST(test_s21_negate_max_values) {
  s21_decimal result;
  int return_code;

  // Maximum positive value
  s21_decimal a = {0};
  a.bits[0] = 0xFFFFFFFF;
  a.bits[1] = 0xFFFFFFFF;
  a.bits[2] = 0xFFFFFFFF;
  a.bits[3] = 0;  // Positive

  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_ne(result.bits[3] & (1u << 31), 0);

  // Maximum negative value
  a.bits[3] = 0x80000000;  // Negative

  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);
}
END_TEST

START_TEST(test_s21_negate_preserves_scale) {
  s21_decimal result;
  int return_code;

  // Test that scale is preserved during negation
  s21_decimal a = create_decimal(12345, 5);  // 0.12345
  unsigned int original_scale = (a.bits[3] >> 16) & 0xFF;

  return_code = s21_negate(a, &result);
  ck_assert_int_eq(return_code, 0);

  unsigned int result_scale = (result.bits[3] >> 16) & 0xFF;
  ck_assert_int_eq(result_scale, original_scale);
  ck_assert_int_eq(result.bits[0], 12345);

  // Sign should be flipped
  ck_assert_int_ne((result.bits[3] & (1u << 31)), (a.bits[3] & (1u << 31)));
}
END_TEST

START_TEST(test_s21_negate_double_negation) {
  s21_decimal result1, result2;
  int return_code;

  // Double negation should return original value
  s21_decimal a = create_decimal(42, 3);

  return_code = s21_negate(a, &result1);
  ck_assert_int_eq(return_code, 0);

  return_code = s21_negate(result1, &result2);
  ck_assert_int_eq(return_code, 0);

  // result2 should be equal to original a
  ck_assert_int_eq(result2.bits[0], a.bits[0]);
  ck_assert_int_eq(result2.bits[1], a.bits[1]);
  ck_assert_int_eq(result2.bits[2], a.bits[2]);
  ck_assert_int_eq(result2.bits[3], a.bits[3]);
}
END_TEST

TCase *s21_negate_tcase(void) {
  TCase *tc = tcase_create("s21_negate");

  tcase_add_test(tc, test_s21_negate_basic);
  tcase_add_test(tc, test_s21_negate_with_scale);
  tcase_add_test(tc, test_s21_negate_null_pointer);
  tcase_add_test(tc, test_s21_negate_large_numbers);
  tcase_add_test(tc, test_s21_negate_max_values);
  tcase_add_test(tc, test_s21_negate_preserves_scale);
  tcase_add_test(tc, test_s21_negate_double_negation);

  return tc;
}