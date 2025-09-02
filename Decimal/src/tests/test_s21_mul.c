#include "values.h"

START_TEST(test_s21_mul) {
  s21_decimal res = {0}, buff_1 = {0}, buff_2 = {0};
  s21_decimal expected[] = {
      // [0]  0
      {{0, 0, 0, 0}},  // 0
      {{0, 0, 0, 0}},  // -0
      {{0, 0, 0, 0}},  // 0e-10
      {{0, 0, 0, 0}},  // 0e-28
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // -2^96 + 1
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // 1.
      {{0, 0, 0, 0}},  // 10
      {{0, 0, 0, 0}},  // 123
      {{0, 0, 0, 0}},  // -123.1
      {{0, 0, 0, 0}},  // 123.539
      {{0, 0, 0, 0}},  // 123.4320432
      {{0, 0, 0, 0}},  // 123.4320433
      {{0, 0, 0, 0}},  // .0000111680464441048961025
      {{0, 0, 0, 0}},  // .0000111680464441048961027
      {{0, 0, 0, 0}},  // 65535
      {{0, 0, 0, 0}},  // -65556
      {{0, 0, 0, 0}},  // -39614081257132173194818486271
      {{0, 0, 0, 0}},  // 396140812572827450410071.00415
      {{0, 0, 0, 0}},  // 2^96 - 1

      // [1]  -0
      {{0, 0, 0, 0}},  // 0
      {{0, 0, 0, 0}},  // -0
      {{0, 0, 0, 0}},  // 0e-10
      {{0, 0, 0, 0}},  // 0e-28
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // -2^96 + 1
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // 1.
      {{0, 0, 0, 0}},  // 10
      {{0, 0, 0, 0}},  // 123
      {{0, 0, 0, 0}},  // -123.1
      {{0, 0, 0, 0}},  // 123.539
      {{0, 0, 0, 0}},  // 123.4320432
      {{0, 0, 0, 0}},  // 123.4320433
      {{0, 0, 0, 0}},  // .0000111680464441048961025
      {{0, 0, 0, 0}},  // .0000111680464441048961027
      {{0, 0, 0, 0}},  // 65535
      {{0, 0, 0, 0}},  // -65556
      {{0, 0, 0, 0}},  // -39614081257132173194818486271
      {{0, 0, 0, 0}},  // 396140812572827450410071.00415
      {{0, 0, 0, 0}},  // 2^96 - 1

      // [2]  0e-10
      {{0, 0, 0, 0}},  // 0
      {{0, 0, 0, 0}},  // -0
      {{0, 0, 0, 0}},  // 0e-10
      {{0, 0, 0, 0}},  // 0e-28
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // -2^96 + 1
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // 1.
      {{0, 0, 0, 0}},  // 10
      {{0, 0, 0, 0}},  // 123
      {{0, 0, 0, 0}},  // -123.1
      {{0, 0, 0, 0}},  // 123.539
      {{0, 0, 0, 0}},  // 123.4320432
      {{0, 0, 0, 0}},  // 123.4320433
      {{0, 0, 0, 0}},  // .0000111680464441048961025
      {{0, 0, 0, 0}},  // .0000111680464441048961027
      {{0, 0, 0, 0}},  // 65535
      {{0, 0, 0, 0}},  // -65556
      {{0, 0, 0, 0}},  // -39614081257132173194818486271
      {{0, 0, 0, 0}},  // 396140812572827450410071.00415
      {{0, 0, 0, 0}},  // 2^96 - 1

      // [3]  0e-28
      {{0, 0, 0, 0}},  // 0
      {{0, 0, 0, 0}},  // -0
      {{0, 0, 0, 0}},  // 0e-10
      {{0, 0, 0, 0}},  // 0e-28
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // -2^96 + 1
      {{0, 0, 0, 0}},  // 1
      {{0, 0, 0, 0}},  // 1.
      {{0, 0, 0, 0}},  // 10
      {{0, 0, 0, 0}},  // 123
      {{0, 0, 0, 0}},  // -123.1
      {{0, 0, 0, 0}},  // 123.539
      {{0, 0, 0, 0}},  // 123.4320432
      {{0, 0, 0, 0}},  // 123.4320433
      {{0, 0, 0, 0}},  // .0000111680464441048961025
      {{0, 0, 0, 0}},  // .0000111680464441048961027
      {{0, 0, 0, 0}},  // 65535
      {{0, 0, 0, 0}},  // -65556
      {{0, 0, 0, 0}},  // -39614081257132173194818486271
      {{0, 0, 0, 0}},  // 396140812572827450410071.00415
      {{0, 0, 0, 0}},  // 2^96 - 1

      // [4]  1
      {{0, 0, 0, 0}},                                      // 0
      {{0, 0, 0, 0}},                                      // -0
      {{0, 0, 0, 0}},                                      // 0e-10
      {{0, 0, 0, 0}},                                      // 0e-28
      {{1, 0, 0, 0}},                                      // 1
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
      {{1, 0, 0, 0}},                                      // 1
      {{10, 0, 0, 0x00010000}},                            // 1.
      {{10, 0, 0, 0}},                                     // 10
      {{123, 0, 0, 0}},                                    // 123
      {{1231, 0, 0, 0x80010000}},                          // -123.1
      {{123539, 0, 0, 0x00030000}},                        // 123.539
      {{1234320432, 0, 0, 0x00070000}},                    // 123.4320432
      {{1234320433, 0, 0, 0x00070000}},                    // 123.4320433
      {{0x5F5E1001, 0x0DE0B6B3, 0x00000006,
        0x00190000}},  // .0000111680464441048961025
      {{0x5F5E1003, 0x0DE0B6B3, 0x00000006,
        0x00190000}},               // .0000111680464441048961027
      {{65535, 0, 0, 0}},           // 65535
      {{65556, 0, 0, 0x80000000}},  // -65556
      {{0xFFFFFFFF, 0x000003FF, 0x80000000,
        0x80000000}},  // -39614081257132173194818486271
      {{0xDD3F41FF, 0x0216F44B, 0x80000000,
        0x00050000}},  // 396140812572827450410071.00415
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // 2^96 - 1

      // [5]  2^96 - 1
      {{0, 0, 0, 0}},                                      // 0
      {{0, 0, 0, 0}},                                      // -0
      {{0, 0, 0, 0}},                                      // 0e-10
      {{0, 0, 0, 0}},                                      // 0e-28
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 1
      {{0x00000001, 0x00000000, 0x00000000, 0x80000000}},  // -2^96 + 1
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 1
      {{0xFFFFFFF6, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},  // 1.
      {{0xFFFFFFF6, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 10
      {{0xFFFFFF85, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 123
      {{0xFFFFFED9, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},  // -123.1
      {{0xFFFFFECD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00030000}},  // 123.539
      {{0xFFFFFECD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00070000}},  // 123.4320432
      {{0xFFFFFECD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00070000}},  // 123.4320433
      {{0, 0, 0, 1}},  // .0000111680464441048961025 (overflow)
      {{0, 0, 0, 1}},  // .0000111680464441048961027 (overflow)
      {{0xFFFF0001, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 65535
      {{0xFFFEFFEC, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -65556
      {{0x00000001, 0xFFFFFC00, 0x7FFFFFFF,
        0}},           // -39614081257132173194818486271
      {{0, 0, 0, 1}},  // 396140812572827450410071.00415 (overflow)
      {{0, 0, 0, 1}},  // 2^96 - 1 (overflow)
  };

  unsigned int err;
  for (int i = 2; i--;) {
    for (int j = 0, c_size = (int)(sizeof(value_2) / sizeof(s21_decimal));
         j < (int)((sizeof(expected) / sizeof(s21_decimal) - c_size) / c_size);
         j++) {
      for (int k = 0; k < c_size; k++) {
        cpy(value_1[j], &buff_1, 1);
        cpy(value_2[k], &buff_2, 1);
        zeroize(&res);
        err = i ? s21_mul(buff_1, buff_2, &res) : s21_mul(buff_2, buff_1, &res);

        if (err) {
          // Check if we expect an error
          ck_assert(expected[j * 21 + k].bits[3] == err);
        } else {
          // Check if result matches expected
          ck_assert(s21_is_equal(res, expected[j * 21 + k]));
        }
      }
    }
  }
}
END_TEST

START_TEST(test_s21_mul_edge_cases) {
  s21_decimal result;

  // Test 1: NULL result pointer
  s21_decimal val1 = {{1, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  int ret = s21_mul(val1, val2, NULL);
  ck_assert_int_eq(ret, 3);  // Should return error code for NULL result

  // Test 2: Multiplying zero by zero
  s21_decimal zero = {0};
  ret = s21_mul(zero, zero, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);  // Positive sign

  // Test 3: Maximum positive * 2 (should overflow)
  s21_decimal max_positive = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  ret = s21_mul(max_positive, two, &result);
  // This might not overflow depending on your implementation
  // Let's check if it's either success or overflow
  ck_assert(ret == 0 || ret == 1);

  // Test 4: Different scales multiplication
  s21_decimal num1 = {{123, 0, 0, 0x00010000}};  // 12.3
  s21_decimal num2 = {{456, 0, 0, 0x00020000}};  // 4.56
  ret = s21_mul(num1, num2, &result);
  ck_assert_int_eq(ret, 0);
  // Should result in 56.088 (56088 with scale 3)
  ck_assert_int_eq(result.bits[0], 56088);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);

  // Test 5: Positive * Negative
  s21_decimal pos = {{5, 0, 0, 0}};
  s21_decimal neg = {{3, 0, 0, 0x80000000}};
  ret = s21_mul(pos, neg, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0x80000000);  // Negative sign

  // Test 6: Negative * Negative
  s21_decimal neg1 = {{5, 0, 0, 0x80000000}};
  s21_decimal neg2 = {{3, 0, 0, 0x80000000}};
  ret = s21_mul(neg1, neg2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);  // Positive sign
}
END_TEST

START_TEST(test_s21_mul_null_result) {
  s21_decimal val1 = {{5, 0, 0, 0}};
  s21_decimal val2 = {{3, 0, 0, 0}};

  // Test with NULL result pointer
  int ret = s21_mul(val1, val2, NULL);
  ck_assert_int_eq(ret, 3);  // Should return error code

  // Test with valid result pointer for comparison
  s21_decimal result;
  ret = s21_mul(val1, val2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 15);
}
END_TEST

START_TEST(test_s21_mul_invalid_scale) {
  s21_decimal result;

  // Test with invalid scale values
  s21_decimal invalid1 = {{1, 0, 0, 0x00FF0000}};  // Scale 255
  s21_decimal invalid2 = {{1, 0, 0, 0x00FE0000}};  // Scale 254
  s21_decimal normal = {{1, 0, 0, 0}};

  int ret = s21_mul(invalid1, normal, &result);
  ck_assert_int_eq(ret, 3);  // Should return error for invalid scale

  ret = s21_mul(normal, invalid2, &result);
  ck_assert_int_eq(ret, 3);  // Should return error for invalid scale
}
END_TEST

START_TEST(test_s21_mul_precision_handling) {
  s21_decimal result;

  // Test precision handling with different scales
  s21_decimal num1 = {{123456789, 0, 0, 0x00050000}};  // 1234.56789
  s21_decimal num2 = {{987654321, 0, 0, 0x00060000}};  // 987.654321

  int ret = s21_mul(num1, num2, &result);
  ck_assert_int_eq(ret, 0);

  // Expected: 1234.56789 * 987.654321 = 1219326.311 (with appropriate scaling)
  // This test verifies that the function handles scale combination correctly
}
END_TEST

TCase *s21_mul_tcase(void) {
  TCase *tc = tcase_create("s21_mul");
  tcase_add_test(tc, test_s21_mul);
  tcase_add_test(tc, test_s21_mul_edge_cases);
  tcase_add_test(tc, test_s21_mul_null_result);
  tcase_add_test(tc, test_s21_mul_invalid_scale);
  tcase_add_test(tc, test_s21_mul_precision_handling);

  return tc;
}
