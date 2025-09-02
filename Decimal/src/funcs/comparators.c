#include "../s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  if (get_exp(value_1) > 28 || get_exp(value_2) > 28) {
    return 1;
  }
  int res = 0;
  if (get_sign(value_1) ^ get_sign(value_2)) {  // The signs are not same.
    res = value_1.bits[3] >> 31 &&
          (!is_zero(value_1) ||
           !is_zero(value_2));  // res = 1 if value_1 is negative.
  } else {
    normalize(&value_1, &value_2);
    for (int i = DEC_SIZE;
         i-- && !res &&
         (get_sign(value_1) ? value_1.bits[i] >= value_2.bits[i]
                            : value_1.bits[i] <= value_2.bits[i]);) {
      res = get_sign(value_1) ? value_1.bits[i] > value_2.bits[i]
                              : value_1.bits[i] < value_2.bits[i];
    }
  }
  return res;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  if (get_exp(value_1) > 28 || get_exp(value_2) > 28) {
    return 1;
  }
  int result = 0;

  s21_decimal tmp_1 = delete_zeros(value_1);
  s21_decimal tmp_2 = delete_zeros(value_2);

  if (get_sign(tmp_1) != get_sign(tmp_2)) {
    result = is_zero(tmp_1) && is_zero(tmp_2);
  } else {
    normalize_decimals(&tmp_1, &tmp_2);
    result = is_mantissa_equal(tmp_1, tmp_2);
  }

  return result;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}