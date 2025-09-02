#include "../s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int buff = 0;
  if (!result || get_exp(value_1) > 28 || get_exp(value_2) > 28) {
    buff = 1;
  } else if (get_sign(value_1) ^ get_sign(value_2)) {
    set_sign(&value_2, get_sign(value_1));
    buff = s21_add(value_1, value_2, result);
  } else if (get_sign(value_1) ? s21_is_less(value_2, value_1)
                               : s21_is_less(value_1, value_2)) {
    sub(value_2, value_1, result);
    set_sign(result, !get_sign(value_1));
  } else {
    sub(value_1, value_2, result);
  }
  return buff;
}

void sub(s21_decimal v_1, s21_decimal v_2, s21_decimal *res) {
  int over_dig = normalize(&v_1, &v_2);
  s21_decimal tmp = {{0, 0, 0, v_1.bits[3]}};
  for (int i = 0, j = 0; i < DEC_SIZE * U_SIZE; i++) {
    if (!get_bit(v_1, i) && get_bit(v_2, i)) {  // 0 - 1
      for (j = i + 1; j < DEC_SIZE * U_SIZE && !get_bit(v_1, j);
           set_bit(&v_1, j), j++) {
      }
      v_1.bits[j / U_SIZE] &= ~(1u << j % U_SIZE);
      set_bit(&tmp, i);
    } else {
      tmp.bits[i / U_SIZE] |= (unsigned int)(get_bit(v_1, i) ^ get_bit(v_2, i))
                              << i % U_SIZE;
    }
  }
  if (over_dig && get_bit(tmp, 0)) {
    sub(tmp, (s21_decimal){{1, 0, 0, tmp.bits[3]}}, &tmp);
  }
  cpy(tmp, res, 1);
}