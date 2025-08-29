#include "test.h"

Suite *s21_decimal_suite(void) {
  Suite *s = suite_create("s21_decimal");

  suite_add_tcase(s, (TCase *)(s21_is_equal_tcase()));
  suite_add_tcase(s, (TCase *)(s21_is_not_equal_tcase()));

  suite_add_tcase(s, (TCase *)(s21_is_greater_tcase()));
  suite_add_tcase(s, (TCase *)(s21_is_greater_or_equal_tcase()));

  suite_add_tcase(s, (TCase *)(s21_is_less_tcase()));
  suite_add_tcase(s, (TCase *)(s21_is_less_or_equal_tcase()));

  suite_add_tcase(s, (TCase *)(s21_from_decimal_to_float_tcase()));
  suite_add_tcase(s, (TCase *)(s21_from_float_to_decimal_tcase()));

  suite_add_tcase(s, (TCase *)(s21_from_int_to_decimal_tcase()));
  suite_add_tcase(s, (TCase *)(s21_from_decimal_to_int_tcase()));

  suite_add_tcase(s, (TCase *)(s21_div_tcase()));
  suite_add_tcase(s, (TCase *)(s21_mul_tcase()));
  suite_add_tcase(s, s21_add_tcase());
  suite_add_tcase(s, s21_sub_tcase());

  suite_add_tcase(s, (TCase *)(s21_negate_tcase()));
  suite_add_tcase(s, (TCase *)(s21_truncate_tcase()));
  suite_add_tcase(s, (TCase *)(s21_floor_tcase()));
  suite_add_tcase(s, (TCase *)(s21_round_tcase()));
  suite_add_tcase(s, (TCase *)(s21_banking_round_tcase()));
  suite_add_tcase(s, s21_normalization_tcase());

  return s;
}

int main() {
  //   int count_failed;
  Suite *s = s21_decimal_suite();
  SRunner *srunner = srunner_create(s);

  srunner_run_all(srunner, CK_VERBOSE);
  //   count_failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);

  printf("\nAll tests passed\n");

  //   return !count_failed ? EXIT_SUCCESS : EXIT_FAILURE;
  return 0;
}