#include "../s21_decimal.h"
#include "test.h"

s21_decimal create_decimal(int value, int scale) {
  s21_decimal dec;
  dec.bits[0] = (value < 0) ? (ui_32)(-(long)value) : (ui_32)value;
  dec.bits[1] = 0;
  dec.bits[2] = 0;
  dec.bits[3] = (value < 0) ? (1u << 31) : 0;
  if (scale > 0) {
    dec.bits[3] |= (scale << 16);
  }
  return dec;
}

START_TEST(test_s21_from_decimal_to_int_basic) {
  int result;
  int return_code;
  s21_decimal src;

  // Positive number
  src = create_decimal(123, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 123);

  // Negative number
  src = create_decimal(-456, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, -456);

  // Zero
  src = create_decimal(0, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 0);

  // Max positive int
  src = create_decimal(2147483647, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 2147483647);

  // Min negative int
  src = create_decimal(-2147483647, 0);
  src.bits[0] = 2147483648u;
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, -2147483648);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_with_scale) {
  int result;
  int return_code;
  s21_decimal src;

  // Decimal with scale 1 (12.3 -> 12)
  src = create_decimal(123, 1);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 12);

  // Decimal with scale 2 (4.56 -> 4)
  src = create_decimal(456, 2);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 4);

  // Negative decimal with scale 1 (-7.89 -> -7)
  src = create_decimal(-789, 2);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, -7);

  // Decimal with scale 3 (1000 -> 1)
  src = create_decimal(1000, 3);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_null_pointer) {
  s21_decimal src = create_decimal(123, 0);

  int return_code = s21_from_decimal_to_int(src, NULL);
  ck_assert_int_eq(return_code,
                   1);  // Should return error code for NULL pointer
}
END_TEST
START_TEST(test_s21_from_decimal_to_int_overflow) {
  int result;
  int return_code;
  s21_decimal src = {0};

  // Value larger than INT_MAX
  src.bits[0] = 2147483648u;  // 2^31
  src.bits[3] = 0;            // Positive, scale = 0
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 1);  // Should return error for overflow

  // Value smaller than INT_MIN
  src.bits[0] = 2147483649u;  // -2^31 - 1
  src.bits[3] = 0x80000000;   // Negative, scale = 0
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 1);  // Should return error for underflow
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_edge_cases) {
  int result;
  int return_code;
  s21_decimal src;

  // Small positive number
  src = create_decimal(1, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 1);

  // Small negative number
  src = create_decimal(-1, 0);
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, -1);

  // Rounding down positive
  src = create_decimal(999, 3);  // 0.999 -> 0
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 0);

  // Rounding down negative
  src = create_decimal(-999, 3);  // -0.999 -> 0
  return_code = s21_from_decimal_to_int(src, &result);
  ck_assert_int_eq(return_code, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_multiple_calls) {
  int result1, result2, result3;
  s21_decimal src1, src2, src3;

  src1 = create_decimal(10, 0);
  src2 = create_decimal(-20, 0);
  src3 = create_decimal(30, 1);  // 3.0 -> 3

  s21_from_decimal_to_int(src1, &result1);
  s21_from_decimal_to_int(src2, &result2);
  s21_from_decimal_to_int(src3, &result3);

  ck_assert_int_eq(result1, 10);
  ck_assert_int_eq(result2, -20);
  ck_assert_int_eq(result3, 3);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int_boundary_values) {
  int result;

  const int test_values[] = {1,    -1,   2,     -2,         100,
                             -100, 1000, -1000, 2147483647, -2147483648,
                             0,    999,  -999};

  for (int i = 0; i < 13; i++) {
    int value = test_values[i];

    int return_code =
        s21_from_decimal_to_int(create_decimal(value, 0), &result);

    ck_assert_int_eq(return_code, 0);
    ck_assert_int_eq(result, value);
  }
}
END_TEST

TCase *s21_from_decimal_to_int_tcase(void) {
  TCase *tc = tcase_create("s21_from_decimal_to_int");

  tcase_add_test(tc, test_s21_from_decimal_to_int_basic);
  tcase_add_test(tc, test_s21_from_decimal_to_int_with_scale);
  tcase_add_test(tc, test_s21_from_decimal_to_int_null_pointer);
  tcase_add_test(tc, test_s21_from_decimal_to_int_overflow);
  tcase_add_test(tc, test_s21_from_decimal_to_int_edge_cases);
  tcase_add_test(tc, test_s21_from_decimal_to_int_multiple_calls);
  tcase_add_test(tc, test_s21_from_decimal_to_int_boundary_values);

  return tc;
}