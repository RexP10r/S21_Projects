#include "test.h"

START_TEST(test_float_to_decimal_null_ptr) {
  int ret = s21_from_float_to_decimal(1.5f, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_zero) {
  s21_decimal result;

  // Положительный ноль
  int ret = s21_from_float_to_decimal(0.0f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(is_zero(result), 1);
  ck_assert_int_eq(get_sign(result), 0);

  // Отрицательный ноль
  ret = s21_from_float_to_decimal(-0.0f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(is_zero(result), 1);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_float_to_decimal_special_values) {
  s21_decimal result;

  // NaN
  int ret = s21_from_float_to_decimal(NAN, &result);
  ck_assert_int_eq(ret, 1);

  // Infinity
  ret = s21_from_float_to_decimal(INFINITY, &result);
  ck_assert_int_eq(ret, 1);

  // -Infinity
  ret = s21_from_float_to_decimal(-INFINITY, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_range) {
  s21_decimal result;

  // Слишком большое значение
  int ret = s21_from_float_to_decimal(1e30f, &result);
  ck_assert_int_eq(ret, 1);

  // Слишком маленькое значение (денормализованные числа)
  ret = s21_from_float_to_decimal(1e-30f, &result);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_float_to_decimal_basic) {
  s21_decimal result;

  // Тест 1: 1.0f
  int ret = s21_from_float_to_decimal(1.0f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3] & 0x7FFFFFFF,
                   0);  // Положительное, экспонента 0

  // Тест 2: -1.0f
  ret = s21_from_float_to_decimal(-1.0f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq(result.bits[3], 0x80000000);  // Отрицательное, экспонента 0

  // Тест 3: 123.456f (ожидаем 123456 * 10^-3)
  ret = s21_from_float_to_decimal(123.456f, &result);
  //       float da;
  //   s21_from_decimal_to_float(result, &da);
  //   print_decimal(result);
  //   printf("%f\n", da);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);  // Экспонента 3
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 4: -123.456f
  ret = s21_from_float_to_decimal(-123.456f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);  // Экспонента 3
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0x80000000);  // Отрицательное

  // Тест 5: 0.001f (ожидаем 1 * 10^-3)
  ret = s21_from_float_to_decimal(0.001f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);  // Экспонента 3
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 6: -0.001f
  ret = s21_from_float_to_decimal(-0.001f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);  // Экспонента 3
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0x80000000);  // Отрицательное
}
END_TEST

START_TEST(test_float_to_decimal_precision) {
  s21_decimal result;

  // Тест 1: 1234567.0f (ожидаем 1234567 * 10^0)
  int ret = s21_from_float_to_decimal(1234567.0f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 0);  // Экспонента 0
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 2: 123456.7f (ожидаем 1234567 * 10^-1)
  ret = s21_from_float_to_decimal(123456.7f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 1);  // Экспонента 1
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 3: 12345.67f (ожидаем 1234567 * 10^-2)
  ret = s21_from_float_to_decimal(12345.67f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 2);  // Экспонента 2
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 4: 1234.567f (ожидаем 1234567 * 10^-3)
  ret = s21_from_float_to_decimal(1234.567f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 3);  // Экспонента 3
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 5: 123.4567f (ожидаем 1234567 * 10^-4)
  ret = s21_from_float_to_decimal(123.4567f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 4);  // Экспонента 4
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 6: 12.34567f (ожидаем 1234567 * 10^-5)
  ret = s21_from_float_to_decimal(12.34567f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 5);  // Экспонента 5
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 7: 1.234567f (ожидаем 1234567 * 10^-6)
  ret = s21_from_float_to_decimal(1.234567f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 6);  // Экспонента 6
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное

  // Тест 8: 0.1234567f (ожидаем 1234567 * 10^-7)
  ret = s21_from_float_to_decimal(0.1234567f, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
  ck_assert_int_eq((result.bits[3] >> 16) & 0xFF, 7);  // Экспонента 7
  ck_assert_int_eq(result.bits[3] & 0x80000000, 0);  // Положительное
}
END_TEST

// Тесты для s21_from_decimal_to_float
START_TEST(test_decimal_to_float_null_ptr) {
  s21_decimal dec = {{123, 0, 0, 0}};
  int ret = s21_from_decimal_to_float(dec, NULL);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_decimal_to_float_zero) {
  s21_decimal zero_pos = {{0, 0, 0, 0}};
  s21_decimal zero_neg = {{0, 0, 0, 0x80000000}};
  float result;

  // Положительный ноль
  int ret = s21_from_decimal_to_float(zero_pos, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(result, 0.0f);

  // Отрицательный ноль
  ret = s21_from_decimal_to_float(zero_neg, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert_float_eq(result, -0.0f);
}
END_TEST

START_TEST(test_decimal_to_float_basic) {
  s21_decimal test_cases[] = {
      {{1, 0, 0, 0}},                // 1
      {{1, 0, 0, 0x80000000}},       // -1
      {{123456, 0, 0, 0}},           // 123456
      {{123456, 0, 0, 0x80000000}},  // -123456
      {{123456, 0, 0, 0x00020000}},  // 1234.56
      {{123456, 0, 0, 0x80020000}},  // -1234.56
  };

  float expected[] = {1.0f, -1.0f, 123456.0f, -123456.0f, 1234.56f, -1234.56f};

  for (size_t i = 0; i < sizeof(test_cases) / sizeof(s21_decimal); i++) {
    float result;
    int ret = s21_from_decimal_to_float(test_cases[i], &result);
    ck_assert_int_eq(ret, 0);
    ck_assert_float_eq_tol(result, expected[i], 1e-5f);
  }
}
END_TEST

START_TEST(test_decimal_to_float_edge_cases) {
  s21_decimal max_decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal min_decimal = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  // s21_decimal small_decimal = {{1, 0, 0, 0x001C0000}}; // 1e-28

  float result;

  // Максимальное положительное decimal
  int ret = s21_from_decimal_to_float(max_decimal, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert(result > 0);

  // Минимальное отрицательное decimal
  ret = s21_from_decimal_to_float(min_decimal, &result);
  ck_assert_int_eq(ret, 0);
  ck_assert(result < 0);

  // Очень маленькое значение
  // ret = s21_from_decimal_to_float(small_decimal, &result);
  // ck_assert_int_eq(ret, 1);
  // ck_assert_float_eq_tol(result, 1e-28f, 1e-30f);
}
END_TEST

START_TEST(test_decimal_to_float_rounding) {
  // Проверка округления при преобразовании
  s21_decimal cases[] = {
      {{123456789, 0, 0, 0x00060000}},  // 123.456789
      {{123456789, 0, 0, 0x00070000}},  // 12.3456789
      {{123456789, 0, 0, 0x00080000}},  // 1.23456789
  };

  float expected[] = {123.456789f, 12.3456789f, 1.23456789f};

  for (size_t i = 0; i < sizeof(cases) / sizeof(s21_decimal); i++) {
    float result;
    int ret = s21_from_decimal_to_float(cases[i], &result);
    ck_assert_int_eq(ret, 0);
    ck_assert_float_eq_tol(result, expected[i], 1e-7f);
  }
}
END_TEST

// Создание test cases
TCase *s21_from_float_to_decimal_tcase(void) {
  TCase *tc = tcase_create("s21_from_float_to_decimal");

  tcase_add_test(tc, test_float_to_decimal_null_ptr);
  tcase_add_test(tc, test_float_to_decimal_zero);
  tcase_add_test(tc, test_float_to_decimal_special_values);
  tcase_add_test(tc, test_float_to_decimal_range);
  tcase_add_test(tc, test_float_to_decimal_basic);
  tcase_add_test(tc, test_float_to_decimal_precision);

  return tc;
}

TCase *s21_from_decimal_to_float_tcase(void) {
  TCase *tc = tcase_create("s21_from_decimal_to_float");

  tcase_add_test(tc, test_decimal_to_float_null_ptr);
  tcase_add_test(tc, test_decimal_to_float_zero);
  tcase_add_test(tc, test_decimal_to_float_basic);
  tcase_add_test(tc, test_decimal_to_float_edge_cases);
  tcase_add_test(tc, test_decimal_to_float_rounding);

  return tc;
}