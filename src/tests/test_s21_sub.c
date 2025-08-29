#include "test.h"

START_TEST(test_s21_sub) {
  s21_decimal res = {0};
  s21_sub((s21_decimal){{0xFFFFFFFF, 0x000003FF, 0, 0x00010000}},
          (s21_decimal){{0xDD3F41FF, 0x0216F44B, 0, 0x80020000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0xDD3F41F5, 0x02171C4B, 0, 0x00020000}}));
}
END_TEST

START_TEST(test_s21_sub_basic) {
  s21_decimal res = {0};
  s21_sub((s21_decimal){{0xFFFFFFFF, 0x000003FF, 0, 0x00010000}},
          (s21_decimal){{0xDD3F41FF, 0x0216F44B, 0, 0x80020000}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0xDD3F41F5, 0x02171C4B, 0, 0x00020000}}));
}
END_TEST

START_TEST(test_s21_sub_zero) {
  s21_decimal res = {0};
  // Вычитание нуля
  s21_sub((s21_decimal){{123, 0, 0, 0}}, (s21_decimal){{0, 0, 0, 0}}, &res);
  ck_assert(s21_is_equal(res, (s21_decimal){{123, 0, 0, 0}}));

  // Вычитание из нуля
  s21_sub((s21_decimal){{0, 0, 0, 0}}, (s21_decimal){{456, 0, 0, 0}}, &res);
  ck_assert(s21_is_equal(res, (s21_decimal){{456, 0, 0, 0x80000000}}));  // -456
}
END_TEST

START_TEST(test_s21_sub_max_values) {
  s21_decimal res = {0};
  // MAX_INT - 1
  s21_sub((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
          (s21_decimal){{1, 0, 0, 0}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}}));

  // 1 - MAX_INT
  s21_sub((s21_decimal){{1, 0, 0, 0}},
          (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}, &res);
  ck_assert(s21_is_equal(
      res, (s21_decimal){{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}));
}
END_TEST

START_TEST(test_s21_sub_negative) {
  s21_decimal res = {0};
  // Отрицательное - отрицательное
  s21_sub((s21_decimal){{100, 0, 0, 0x80000000}},
          (s21_decimal){{50, 0, 0, 0x80000000}}, &res);
  ck_assert(s21_is_equal(res, (s21_decimal){{50, 0, 0, 0x80000000}}));  // -50

  // Положительное - отрицательное (эквивалентно сложению)
  s21_sub((s21_decimal){{100, 0, 0, 0}}, (s21_decimal){{50, 0, 0, 0x80000000}},
          &res);
  ck_assert(s21_is_equal(res, (s21_decimal){{150, 0, 0, 0}}));  // 150

  // Отрицательное - положительное
  s21_sub((s21_decimal){{100, 0, 0, 0x80000000}}, (s21_decimal){{50, 0, 0, 0}},
          &res);
  ck_assert(s21_is_equal(res, (s21_decimal){{150, 0, 0, 0x80000000}}));  // -150
}
END_TEST

START_TEST(test_s21_sub_overflow) {
  s21_decimal res = {0};
  // MIN_INT - 1 (должно вернуть ошибку переполнения)
  int status =
      s21_sub((s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
              (s21_decimal){{1, 0, 0, 0}}, &res);
  ck_assert_int_eq(status, 2);  // Должна быть ошибка
}
END_TEST

START_TEST(test_s21_sub_different_scales) {
  s21_decimal res = {0};
  // 123.45 (scale 2) - 67.8 (scale 1)
  s21_sub((s21_decimal){{12345, 0, 0, 0x00020000}},
          (s21_decimal){{678, 0, 0, 0x00010000}}, &res);
  ck_assert(
      s21_is_equal(res, (s21_decimal){{5565, 0, 0, 0x00020000}}));  // 55.65
}
END_TEST

START_TEST(test_s21_sub_null_pointer) {
  // Тест на нулевой указатель результата
  int status = s21_sub((s21_decimal){{100, 0, 0, 0}},
                       (s21_decimal){{50, 0, 0, 0}}, NULL);
  ck_assert_int_eq(status, 1);  // Должна вернуть ошибку
}
END_TEST

START_TEST(test_s21_sub_edge_cases) {
  s21_decimal res = {0};

  // 1 - 0.9999999999999999999999999999
  s21_sub((s21_decimal){{1, 0, 0, 0}},
          (s21_decimal){{0xFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x001C0000}}, &res);
  // Результат должен быть очень маленьким положительным числом

  // Граничные значения с разными знаками
  s21_sub((s21_decimal){{0x7FFFFFFF, 0, 0, 0}},           // INT_MAX
          (s21_decimal){{0x80000000, 0, 0, 0x80000000}},  // -INT_MIN
          &res);
  // Результат: INT_MAX + INT_MIN = -1 (из-за переполнения)
}
END_TEST

TCase *s21_sub_tcase(void) {
  TCase *tc = tcase_create("s21_sub");
  tcase_add_test(tc, test_s21_sub);
  tcase_add_test(tc, test_s21_sub_basic);
  tcase_add_test(tc, test_s21_sub_zero);
  tcase_add_test(tc, test_s21_sub_max_values);
  tcase_add_test(tc, test_s21_sub_negative);
  tcase_add_test(tc, test_s21_sub_overflow);
  tcase_add_test(tc, test_s21_sub_different_scales);
  tcase_add_test(tc, test_s21_sub_null_pointer);
  tcase_add_test(tc, test_s21_sub_edge_cases);

  return tc;
}