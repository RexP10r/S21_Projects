#include "test.h"

static s21_decimal div_values_1[] = {
    {{10, 0, 0, 0}},                            // 10
    {{10, 0, 0, 0}},                            // 10
    {{1, 0, 0, 0}},                             // 1
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // MAX
    {{0, 0, 0, 0}},                             // 0
    {{10, 0, 0, 0x80000000}},                   // -10
    {{100, 0, 0, 0x00010000}},                  // 10.0
    {{1, 0, 0, 0x001C0000}},                    // 1, exp 28
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // MAX
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // MAX
};

static s21_decimal div_values_2[] = {
    {{0, 0, 0, 0}},           // 0
    {{2, 0, 0, 0}},           // 2
    {{3, 0, 0, 0}},           // 3
    {{1, 0, 0, 0}},           // 1
    {{5, 0, 0, 0}},           // 5
    {{2, 0, 0, 0}},           // 2
    {{5, 0, 0, 0}},           // 5
    {{3, 0, 0, 0}},           // 3
    {{1, 0, 0, 0x001C0000}},  // 1 max exp
    {{1, 0, 0, 0x801C0000}},  // -1 max exp
};

static int expected_codes[] = {
    3,  // div by zero
    0,  // 10/2=5
    0,  // 1/3=0.333..
    0,  // max/1=max
    0,  // 0/5=0
    0,  // -10/2=-5
    0,  // 10.0/5=2.0
    0,  // 1/3 with max exp
    1,  // max/1 with max exp
    2,  // max/-1 with max exp
};

static s21_decimal expected_results[] = {
    {{0}},                                      // Not used for error
    {{5, 0, 0, 0}},                             // 5
    {{0}},                                      // Will check properties
    {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // MAX
    {{0, 0, 0, 0}},                             // 0
    {{5, 0, 0, 0x80000000}},                    // -5
    {{20, 0, 0, 0x00010000}},                   // 2.0
    {{0}},                                      // Should be zero
    {{0}},                                      // Should be zero
    {{0}},                                      // Should be zero
};

START_TEST(test_s21_div) {
  s21_decimal result = {0};
  int ret = s21_div(div_values_1[_i], div_values_2[_i], &result);

  ck_assert_int_eq(ret, expected_codes[_i]);

  if (ret == 0) {
    switch (_i) {
      case 2:
        ck_assert_int_eq(get_exp(result), 28);
        ck_assert(!is_zero(result));
        break;
      case 6:
        ck_assert_int_eq(get_exp(result), 1);
        break;
      case 7:
        ck_assert(is_zero(result));
        break;
      default:
        for (int j = 0; j < 4; j++) {
          ck_assert_int_eq(result.bits[j], expected_results[_i].bits[j]);
        }
    }
  }
}
END_TEST

TCase *s21_div_tcase(void) {
  TCase *tc = tcase_create("s21_div");
  tcase_add_loop_test(tc, test_s21_div, 0,
                      sizeof(div_values_1) / sizeof(s21_decimal));
  return tc;
}