#include "values.h"

START_TEST(test_s21_add) {
  s21_decimal
      res = {0},
      buff_1 = {0}, buff_2 = {0},
      expected[] = {
          // [0]  0
          {{0, 0, 0, 0}},                                      // 0
          {{0, 0, 0, 0}},                                      // -0
          {{0, 0, 0, 0}},                                      // -0e10
          {{0, 0, 0, 0}},                                      // -0e28
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
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 2^96 - 1
                                                               // [1]  -0
          {{0, 0, 0, 0}},                                      // 0
          {{0, 0, 0, 0}},                                      // -0
          {{0, 0, 0, 0}},                                      // -0e10
          {{0, 0, 0, 0}},                                      // -0e28
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
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 2^96 - 1
                                                               // [2]  -0e10
          {{0, 0, 0, 0}},                                      // 0
          {{0, 0, 0, 0}},                                      // -0
          {{0, 0, 0, 0}},                                      // -0e10
          {{0, 0, 0, 0}},                                      // -0e28
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
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 2^96 - 1
                                                               // [3]  -0e28
          {{0, 0, 0, 0}},                                      // 0
          {{0, 0, 0, 0}},                                      // -0
          {{0, 0, 0, 0}},                                      // -0e10
          {{0, 0, 0, 0}},                                      // -0e28
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
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 2^96 - 1
                                                               // [4]  1
          {{1, 0, 0, 0}},                                      // 0
          {{1, 0, 0, 0}},                                      // -0
          {{1, 0, 0, 0}},                                      // -0e10
          {{1, 0, 0, 0}},                                      // -0e28
          {{2, 0, 0, 0}},                                      // 1
          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
          {{2, 0, 0, 0}},                                      // 1
          {{20, 0, 0, 0x00010000}},                            // 1.
          {{11, 0, 0, 0}},                                     // 10
          {{124, 0, 0, 0}},                                    // 123
          {{1221, 0, 0, 0x80010000}},                          // -123.1
          {{124539, 0, 0, 0x00030000}},                        // 123.539
          {{1244320432, 0, 0, 0x00070000}},                    // 123.4320432
          {{1244320433, 0, 0, 0x00070000}},                    // 123.4320433
          {{0xA95E1001, 0x23F4B7FB, 0x0008459B,
            0x00190000}},  // .0000111680464441048961025
          {{0xA95E1003, 0x23F4B7FB, 0x0008459B,
            0x00190000}},               // .0000111680464441048961027
          {{65536, 0, 0, 0}},           // 65535
          {{65555, 0, 0, 0x80000000}},  // -65556
          {{0xFFFFFFFE, 0x000003FF, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDD40C89F, 0x0216F44B, 0x80000000,
            0x00050000}},  // 396140812572827450410071.00415
          {{0, 0, 0, 1}},  // 2^96 - 1   ERROR - TOO BIG.
                           // [5]  2^96 - 1
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // 0
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // -0
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // -0e10
          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // -0e28
          {{0, 0, 0, 1}},                             // 1   ERROR - TOO BIG.
          {{0, 0, 0, 0}},                             // -2^96 + 1
          {{0, 0, 0, 1}},                             // 1   ERROR - TOO BIG.
          {{0, 0, 0, 1}},                             // 1.   ERROR - TOO BIG.
          {{0, 0, 0, 1}},                             // 10   ERROR - TOO BIG.
          {{0, 0, 0, 1}},                             // 123   ERROR - TOO BIG.
          {{0xFFFFFF84, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // -123.1
          {{0, 0, 0, 1}},  // 123.539   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // 123.4320432   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // 123.4320433   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // .0000111680464441048961025   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // .0000111680464441048961027   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // 65535   ERROR - TOO BIG.
          {{0xFFFEFFEB, 0xFFFFFFFF, 0xFFFFFFFF, 0}},  // -65556
          {{0, 0xFFFFFC00, 0x7FFFFFFF,
            0}},           // 2^96 - 1 + -39614081257132173194818486271 =
                           // 39614081257132164398725464064
          {{0, 0, 0, 1}},  // 396140812572827450410071.00415   ERROR - TOO BIG.
          {{0, 0, 0, 1}},  // 2^96 - 1   ERROR - TOO BIG.
                           // [6]  -1
          {{1, 0, 0, 0x80000000}},           // 0
          {{1, 0, 0, 0x80000000}},           // -0
          {{1, 0, 0, 0x80000000}},           // 0e-10
          {{1, 0, 0, 0x80000000}},           // 0e-28
          {{0, 0, 0, 0}},                    // 1
          {{0, 0, 0, 2}},                    // -2^96 + 1   ERROR - TOO small.
          {{0, 0, 0, 0}},                    // 1
          {{0, 0, 0, 0}},                    // 1.
          {{9, 0, 0, 0}},                    // 10
          {{122, 0, 0, 0}},                  // 123
          {{1241, 0, 0, 0x80010000}},        // -123.1
          {{122539, 0, 0, 0x00030000}},      // 123.539
          {{1224320432, 0, 0, 0x00070000}},  // 123.4320432
          {{1224320433, 0, 0, 0x00070000}},  // 123.4320433
          {{0xEAA1EFFF, 0x08334A94, 0x0008458F,
            0x80190000}},  // -1 + .0000111680464441048961025 =
                           // -.9999888319535558951038975
          {{0xEAA1EFFD, 0x08334A94, 0x0008458F,
            0x80190000}},               // -1 + .0000111680464441048961027 =
                                        // -.9999888319535558951038973
          {{65534, 0, 0, 0}},           // 65535
          {{65557, 0, 0, 0x80000000}},  // -65556
          {{0, 0x00000400, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDD3DBB5F, 0x0216F44B, 0x80000000,
            0x00050000}},  // 396140812572827450410071.00415
          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}},           // 2^96 - 1
                                                               // [7]  .1
          {{1, 0, 0, 0x00010000}},                             // 0
          {{1, 0, 0, 0x00010000}},                             // -0
          {{1, 0, 0, 0x00010000}},                             // -0e10
          {{1, 0, 0, 0x00010000}},                             // -0e28
          {{11, 0, 0, 0x00010000}},                            // 1
          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
          {{11, 0, 0, 0x00010000}},                            // 1
          {{11, 0, 0, 0x00010000}},                            // 1.
          {{101, 0, 0, 0x00010000}},                           // 10
          {{1231, 0, 0, 0x00010000}},                          // 123
          {{1230, 0, 0, 0x80010000}},                          // -123.1
          {{123639, 0, 0, 0x00030000}},                        // 123.539
          {{1235320432, 0, 0, 0x00070000}},                    // 123.4320432
          {{1235320433, 0, 0, 0x00070000}},                    // 123.4320433
          {{0x005E1001, 0x29AF83A1, 0x0000D3C8,
            0x00190000}},  // .0000111680464441048961025
          {{0x005E1003, 0x29AF83A1, 0x0000D3C8,
            0x00190000}},                // .0000111680464441048961027
          {{655351, 0, 0, 0x00010000}},  // 65535
          {{655559, 0, 0, 0x80010000}},  // -65556
          {{0xFFFFFFFE, 0x000003FF, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDD3F690F, 0x0216F44B, 0x80000000,
            0x00050000}},   // 396140812572827450410071.00415
          {{0, 0, 0, 1}},   // 2^96 - 1   ERROR - TOO BIG.
                            // [8]  10
          {{10, 0, 0, 0}},  // 0
          {{10, 0, 0, 0}},  // -0
          {{10, 0, 0, 0}},  // -0e10
          {{10, 0, 0, 0}},  // -0e28
          {{11, 0, 0, 0}},  // 1
          {{0xFFFFFFF5, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
          {{11, 0, 0, 0}},                                     // 1
          {{110, 0, 0, 0x00010000}},                           // 1.
          {{20, 0, 0, 0}},                                     // 10
          {{133, 0, 0, 0}},                                    // 123
          {{1131, 0, 0, 0x80010000}},                          // -123.1
          {{133539, 0, 0, 0x00030000}},                        // 123.539
          {{1334320432, 0, 0, 0x00070000}},                    // 123.4320432
          {{1334320433, 0, 0, 0x00070000}},                    // 123.4320433
          {{0x435E1001, 0xEAA8C386, 0x0052B7D8,
            0x00190000}},  // .0000111680464441048961025
          {{0x435E1003, 0xEAA8C386, 0x0052B7D8,
            0x00190000}},               // .0000111680464441048961027
          {{65545, 0, 0, 0}},           // 65535
          {{65546, 0, 0, 0x80000000}},  // -65556
          {{0xFFFFFFF5, 0x000003FF, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDD4E843F, 0x0216F44B, 0x80000000,
            0x00050000}},    // 396140812572827450410071.00415
          {{0, 0, 0, 1}},    // 2^96 - 1   ERROR - TOO BIG.
                             // [9]  123
          {{123, 0, 0, 0}},  // 0
          {{123, 0, 0, 0}},  // -0
          {{123, 0, 0, 0}},  // -0e10
          {{123, 0, 0, 0}},  // -0e28
          {{124, 0, 0, 0}},  // 1
          {{0xFFFFFF84, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
          {{124, 0, 0, 0}},                                    // 1
          {{1240, 0, 0, 0x00010000}},                          // 1.
          {{133, 0, 0, 0}},                                    // 10
          {{246, 0, 0, 0}},                                    // 123
          {{1, 0, 0, 0x80010000}},                             // -123.1
          {{246539, 0, 0, 0x00030000}},                        // 123.539
          {{2464320432, 0, 0, 0x00070000}},                    // 123.4320432
          {{2464320433, 0, 0, 0x00070000}},                    // 123.4320433
          {{0xED5E1001, 0xA97D546E, 0x03F96EA7,
            0x00190000}},  // .0000111680464441048961025
          {{0xED5E1003, 0xA97D546E, 0x03F96EA7,
            0x00190000}},               // .0000111680464441048961027
          {{65658, 0, 0, 0}},           // 65535
          {{65433, 0, 0, 0x80000000}},  // -65556
          {{0xFFFFFF84, 0x000003FF, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDDFAF0DF, 0x0216F44B, 0x80000000,
            0x00050000}},              // 396140812572827450410071.00415
          {{0, 0, 0, 1}},              // 2^96 - 1   ERROR - TOO BIG.
                                       // [10]  123.1
          {{1231, 0, 0, 0x00010000}},  // 0
          {{1231, 0, 0, 0x00010000}},  // -0
          {{1231, 0, 0, 0x00010000}},  // -0e10
          {{1231, 0, 0, 0x00010000}},  // -0e28
          {{1241, 0, 0, 0x00010000}},  // 1
          {{0xFFFFFF84, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},  // -2^96 + 1
          {{1241, 0, 0, 0x00010000}},                          // 1
          {{1241, 0, 0, 0x00010000}},                          // 1.
          {{1331, 0, 0, 0x00010000}},                          // 10
          {{2461, 0, 0, 0x00010000}},                          // 123
          {{0, 0, 0, 0}},                                      // -123.1
          {{246639, 0, 0, 0x00030000}},                        // 123.539
          {{2465320432, 0, 0, 0x00070000}},                    // 123.4320432
          {{2465320433, 0, 0, 0x00070000}},                    // 123.4320433
          {{0x8E5E1001, 0xC54C215C, 0x03FA4269,
            0x00190000}},  // .0000111680464441048961025
          {{0x8E5E1003, 0xC54C215C, 0x03FA4269,
            0x00190000}},                // .0000111680464441048961027
          {{656581, 0, 0, 0x00010000}},  // 65535
          {{654329, 0, 0, 0x80010000}},  // -65556
          {{0xFFFFFF84, 0x000003FF, 0x80000000,
            0x80000000}},  // -39614081257132173194818486271
          {{0xDDFB17EF, 0x0216F44B, 0x80000000,
            0x00050000}},  // 396140812572827450410071.00415
          {{0, 0, 0, 1}},  // 2^96 - 1   ERROR - TOO BIG.
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
        err = i ? s21_add(buff_1, buff_2, &res) : s21_add(buff_2, buff_1, &res);
        ck_assert(err ? expected[j * 21 + k].bits[3] == err
                      : is_mantissa_equal(res, expected[j * 21 + k]) &&
                            res.bits[3] == expected[j * 21 + k].bits[3]);
      }
    }
  }
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020000}},
          (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00020000}},
          &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x33333332, 0x33333333, 0x33333333, 0x00010000}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{1, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999A, 0x99999999, 0x19999999, 0}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{9, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999A, 0x99999999, 0x19999999, 0}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{11, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999A, 0x99999999, 0x19999999, 0}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{19, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999C, 0x99999999, 0x19999999, 0}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{21, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999C, 0x99999999, 0x19999999, 0}}));
  zeroize(&res);
  s21_add((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00010000}},
          (s21_decimal){{29, 0, 0, 0x00010000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0x9999999C, 0x99999999, 0x19999999, 0}}));
}
END_TEST

START_TEST(test_s21_add_edge_cases) {
  s21_decimal result;

  // Test 1: NULL result pointer
  s21_decimal val1 = {{1, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  int ret = s21_add(val1, val2, NULL);
  ck_assert_int_ne(ret, 0);  // Should return error code for NULL result

  // Test 2: Adding zero to zero
  s21_decimal zero = {0};
  s21_add(zero, zero, &result);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & (1u << 31), 0);  // Positive sign

  // Test 3: Maximum positive + 1 (should overflow)
  s21_decimal max_positive = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one = {{1, 0, 0, 0}};
  ret = s21_add(max_positive, one, &result);
  ck_assert_int_ne(ret, 0);  // Should return overflow error

  // Test 4: Minimum negative - 1 (should underflow)
  s21_decimal min_negative = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal minus_one = {{1, 0, 0, 0x80000000}};
  ret = s21_add(min_negative, minus_one, &result);
  ck_assert_int_ne(ret, 0);  // Should return underflow error

  // Test 5: Different scales addition
  s21_decimal num1 = {{123, 0, 0, 0x00010000}};  // 12.3
  s21_decimal num2 = {{456, 0, 0, 0x00020000}};  // 4.56
  ret = s21_add(num1, num2, &result);
  ck_assert_int_eq(ret, 0);
  // Should result in 16.86 (1686 with scale 2)
  ck_assert_int_eq(result.bits[0], 1686);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 2);

  // Test 6: Positive infinity-like behavior
  s21_decimal huge1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal huge2 = {{2, 0, 0, 0}};
  ret = s21_add(huge1, huge2, &result);
  ck_assert_int_ne(ret, 0);  // Should overflow

  // Test 7: Negative infinity-like behavior
  s21_decimal huge_neg1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal huge_neg2 = {{2, 0, 0, 0x80000000}};
  ret = s21_add(huge_neg1, huge_neg2, &result);
  ck_assert_int_ne(ret, 0);  // Should underflow
}
END_TEST

START_TEST(test_s21_add_extreme_values) {
  s21_decimal result;
  int ret;

  // Test 1: Maximum value with different scales
  s21_decimal max_val = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal small_val = {{1, 0, 0, 0x001C0000}};  // 1e-28

  ret = s21_add(max_val, small_val, &result);
  ck_assert_int_ne(ret, 0);  // Should still overflow

  // Test 2: Very small numbers that should normalize
  s21_decimal tiny1 = {{1, 0, 0, 0x001C0000}};  // 1e-28
  s21_decimal tiny2 = {{1, 0, 0, 0x001C0000}};  // 1e-28

  ret = s21_add(tiny1, tiny2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 28);

  // Test 3: Mixed signs with extreme values
  s21_decimal pos_extreme = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal neg_extreme = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};

  ret = s21_add(pos_extreme, neg_extreme, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 0);  // Should be zero
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);

  // Test 4: Borderline overflow case
  s21_decimal almost_max = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal one_more = {{1, 0, 0, 0}};

  ret = s21_add(almost_max, one_more, &result);
  ck_assert_int_eq(ret, 0);  // Should succeed
  ck_assert_int_eq(result.bits[0], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[1], 0xFFFFFFFF);
  ck_assert_int_eq(result.bits[2], 0xFFFFFFFF);
}
END_TEST

START_TEST(test_s21_add_null_result) {
  s21_decimal val1 = {{5, 0, 0, 0}};
  s21_decimal val2 = {{3, 0, 0, 0}};

  // Test with NULL result pointer
  int ret = s21_add(val1, val2, NULL);
  ck_assert_int_ne(ret, 0);  // Should return error code

  // Test with valid result pointer for comparison
  s21_decimal result;
  ret = s21_add(val1, val2, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 8);
}
END_TEST

START_TEST(test_s21_add_nan_inf_handling) {
  s21_decimal result;

  // Test 1: Attempt to create NaN-like values
  s21_decimal weird1 = {{0, 0, 0, 0x7F000000}};  // Invalid scale
  s21_decimal normal = {{1, 0, 0, 0}};

  int ret = s21_add(weird1, normal, &result);
  // Should either handle gracefully or return error
  ck_assert_int_eq(ret, 1);  // Either valid result or error is acceptable

  s21_decimal invalid1 = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FF0000}};  // Scale 255
  s21_decimal invalid2 = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00FE0000}};  // Scale 254

  ret = s21_add(invalid1, invalid2, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

TCase *s21_add_tcase(void) {
  TCase *tc = tcase_create("s21_add");
  tcase_add_test(tc, test_s21_add);
  tcase_add_test(tc, test_s21_add_edge_cases);
  tcase_add_test(tc, test_s21_add_extreme_values);
  tcase_add_test(tc, test_s21_add_null_result);
  tcase_add_test(tc, test_s21_add_nan_inf_handling);

  return tc;
}