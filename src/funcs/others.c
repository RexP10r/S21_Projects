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
      for (int i = 0; i < scale; ++i)
        div_mod(value, divisor, &value, &remainder);
      set_sign(&value, sign);
      *result = value;
    }
  }
  return fl;
}

int s21_round(s21_decimal value, s21_decimal *result) {
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
      for (int i = 0; i < scale; ++i) {
        div_mod(value, divisor, &value, &remainder);
      }
      set_sign(&value, sign);
      s21_decimal tmp = {{5, 0, 0, 0}};
      int diff = remainder.bits[0] - tmp.bits[0];
      if (diff >= 0) {
        tmp.bits[0] = 1;
        set_sign(&tmp, get_sign(value));
        add(value, tmp, &value);
      }
      *result = value;
    }
  }
  return fl;
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
        if (remainder.bits[0] > 0) count_rem = 1;
      }
      if (sign && count_rem) {
        remainder.bits[0] = 1;
        add(value, remainder, &value);
      }
      set_sign(&value, sign);
      *result = value;
    }
  }
  return fl;
}
