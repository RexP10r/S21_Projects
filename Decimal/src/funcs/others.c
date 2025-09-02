#include "../s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  int fl = 0;
  if (result == NULL || get_exp(value) > 28) {
    fl = 1;
  } else {
    *result = value;
    if (value.bits[3] & 0x80000000) {
      result->bits[3] = value.bits[3] & ~0x80000000;
    } else {
      result->bits[3] = value.bits[3] | 0x80000000;
    }
  }
  return fl;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int err = 1;
  if (!validate(value) && result) {
    *result = value;
    int exp = get_exp(value);
    while (exp-- > 0) {
      reduce_by_10(result);
    }
    set_exp(result, 0);
    err = 0;
  }
  return err;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (is_zero(value)) {
    *result = (s21_decimal){{0}};
    set_sign(result, get_sign(value));
    return 0;
  }
  int err = 1;
  if (!validate(value) && result) {
    int exp = get_exp(value);
    if (exp > 0) {
      s21_decimal pos_half = {{5, 0, 0, 0x00010000}},
                  neg_half = {{5, 0, 0, 0x80010000}}, one = {{1, 0, 0, 0}},
                  fraction;
      s21_truncate(value, result);
      s21_sub(value, *result, &fraction);
      if (s21_is_greater_or_equal(fraction, pos_half) ||
          s21_is_less_or_equal(fraction, neg_half)) {
        get_sign(value) ? s21_sub(*result, one, result)
                        : s21_add(*result, one, result);
      }
      set_exp(result, 0);
      err = 0;
    } else {
      *result = value;
      err = 0;
    }
  }
  return err;
}

int s21_banking_round(s21_decimal value, s21_decimal *result) {
  int fl = 0;
  if (result == NULL || get_exp(value) > 28) {
    fl = 1;
  } else {
    int scale = get_exp(value);
    if (scale == 0) {
      *result = value;
    } else {
      s21_decimal divisor = {{10, 0, 0, 0}};
      set_exp(&value, 0);
      int sign = get_sign(value);
      set_sign(&value, 0);
      s21_decimal remainder = {0};
      int count_rem = 0;
      for (int i = 0; i < scale; ++i) {
        div_mod(value, divisor, &value, &remainder);
        count_rem += remainder.bits[0];
      }
      set_sign(&value, sign);
      int diff = count_rem - 5;
      int checkbit = value.bits[0] & 0x00000001;
      s21_decimal tmp = {0};
      if ((diff == 0 && remainder.bits[0] == 5 && checkbit == 1) ||
          remainder.bits[0] > 5 || (remainder.bits[0] == 5 && diff > 0)) {
        tmp.bits[0] = 1;
        set_sign(&tmp, get_sign(value));
        add(value, tmp, &value);
      }
      *result = value;
    }
  }
  return fl;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (is_zero(value)) {
    *result = (s21_decimal){{0}};
    set_sign(result, get_sign(value));
    return 0;
  }
  int err = 1;
  if (!validate(value) && result) {
    int sign = get_sign(value);
    int exp = get_exp(value);
    if (exp > 0) {
      set_sign(&value, 0);

      s21_decimal zero = {{0, 0, 0, 0x00010000}}, one = {{1, 0, 0, 0}},
                  fraction;
      s21_truncate(value, result);
      s21_sub(value, *result, &fraction);

      if (s21_is_greater(fraction, zero)) {
        if (sign) {
          s21_add(*result, one, result);
        }
      }
      set_exp(result, 0);
      set_sign(result, sign);
      err = 0;
    } else {
      *result = value;
      err = 0;
    }
  }
  return err;
}
