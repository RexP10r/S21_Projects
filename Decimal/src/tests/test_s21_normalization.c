#include "values.h"

START_TEST(test_raise) {
  s21_decimal buff = {0},
              expected_result[] = {
                  {{0, 0, 0, 0}},                             // 0
                  {{0, 0, 0, 0}},                             // +0
                  {{0, 0, 0, 0x00030000}},                    // 0.000
                  {{0, 0, 0, 0x801C0000}},                    // -0e-28
                  {{10, 0, 0, 0}},                            // 1
                  {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // 2^96
                  {{10, 0, 0, 0x80000000}},                   // -1
                  {{10, 0, 0, 0x00010000}},                   // 0.1
                  {{100, 0, 0, 0}},                           // 10
                  {{1230, 0, 0, 0}},                          // 123
                  {{12310, 0, 0, 0x00010000}},                // 123.1
                  {{1235390, 0, 0, 0x00030000}},              // 123.539
                  {{0xDFB659EA, 0x00000002, 0, 0x00070000}},  // 123.4320433
                  {{0xDFB659F4, 0x00000002, 0, 0x00070000}},  // 123.4320434
                  {{0xB9ACA00A, 0x8AC72301, 0x0000003C,
                    0x00190000}},  // 111.680464441048961025
                  {{0xB9ACA014, 0x8AC72301, 0x0000003C,
                    0x00190000}},                    // 111.680464441048961026
                  {{655350, 0, 0, 0}},               // 65535
                  {{655560, 0, 0, 0}},               // 65556
                  {{0xFFFFFFF6, 0x000027FF, 0, 0}},  // 4398046511103
                  {{0xA47893F6, 0x14E58AF6, 0,
                    0x80050000}},  // -1505762442351.25247
                  {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                    0}},  // 79228162514264337593543950335
              };
  for (int i = sizeof(value_1) / sizeof(s21_decimal); i--;) {
    cpy(value_1[i], &buff, 1);
    raise_by_10(&buff);
    ck_assert(is_mantissa_equal(buff, expected_result[i]));
  }
}
END_TEST

START_TEST(test_reduce) {
  s21_decimal buff = {0},
              expected_result[] = {
                  {{0, 0, 0, 0}},                                      // 0
                  {{0, 0, 0, 0}},                                      // +0
                  {{0, 0, 0, 0}},                                      // 0.000
                  {{0, 0, 0, 0}},                                      // -0e-28
                  {{0, 0, 0, 0}},                                      // 1
                  {{0x99999999, 0x99999999, 0x19999999, 0x00010000}},  // 2^96
                  {{0, 0, 0, 0}},                                      // -1
                  {{0, 0, 0, 0}},                                      // 0.1
                  {{1, 0, 0, 0x00010000}},                             // 10 [8]
                  {{12, 0, 0, 0x00010000}},                            // 123
                  {{123, 0, 0, 0x00010000}},                           // 123.1
                  {{12353, 0, 0, 0x00030000}},      // 123.539
                  {{123432043, 0, 0, 0x00070000}},  // 123.4320433
                  {{123432043, 0, 0, 0x00070000}},  // 123.4320434
                  {{0xEFEFCE66, 0x9AFCDF11, 0,
                    0x00190000}},  // .0000111680464441048961025
                  {{0xEFEFCE66, 0x9AFCDF11, 0,
                    0x00190000}},              // .0000111680464441048961026
                  {{6553, 0, 0, 0x00010000}},  // 65535
                  {{6555, 0, 0, 0x00010000}},  // 65556
                  {{0x66666666, 0x00000066, 0, 0x00010000}},  // 4398046511103
                  {{0x62ECB9CC, 0x00357ED4, 0,
                    0x80050000}},  // -1505762442351.25247
                  {{0x99999999, 0x99999999, 0x19999999,
                    0x00010000}},  // 79228162514264337593543950335
              };
  for (int i = sizeof(value_1) / sizeof(s21_decimal); i--;) {
    cpy(value_1[i], &buff, 1);
    if (!get_exp(buff)) {
      set_exp(&buff, 1);
    }
    reduce_by_10(&buff);
    ck_assert(s21_is_equal(buff, expected_result[i]));
  }
}
END_TEST

START_TEST(test_normalize) {
  s21_decimal buff_1 = {0}, buff_2 = {0};
  for (int i = sizeof(value_1) / sizeof(s21_decimal); i--;) {
    cpy(value_1[i], &buff_1, 1);
    cpy(value_2[i], &buff_2, 1);
    normalize(&buff_1, &buff_2);
    ck_assert(get_exp(buff_1) == get_exp(buff_2) || is_zero(buff_1) ||
              is_zero(buff_1));
  }
}
END_TEST

TCase *s21_normalization_tcase(void) {
  TCase *tc = tcase_create("normalization");
  tcase_add_test(tc, test_raise);
  tcase_add_test(tc, test_reduce);
  tcase_add_test(tc, test_normalize);

  return tc;
}