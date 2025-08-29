#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result || get_exp(value_1) > 28 || get_exp(value_2) > 28) {
    return 1;
  }
  int buff = 0;
  if (get_sign(value_1) ^ get_sign(value_2)) {
    set_sign(&value_2, get_sign(value_1));
    buff = s21_sub(value_1, value_2, result);
  } else if (!(buff = is_max(value_1, value_2) || is_max(value_2, value_1))) {
    buff = add(value_1, value_2, result);
  }
  if (buff && get_sign(value_1)) {
    buff = 2;
  } else if (is_zero(*result)) {
    set_sign(result, 0);
    set_exp(result, 0);
  }
  return buff;
}

int add(s21_decimal v_1, s21_decimal v_2, s21_decimal *res) {
  int buff = 0, over_dig = normalize(&v_1, &v_2);
  s21_decimal tmp = {{0, 0, 0, v_1.bits[3]}};
  for (int i = 0; i < DEC_SIZE * U_SIZE; i++) {
    tmp.bits[i / U_SIZE] |=
        (unsigned int)(get_bit(v_1, i) ^ get_bit(v_2, i) ^ buff) << i % U_SIZE;
    buff = get_bit(v_1, i) + get_bit(v_2, i) + buff > 1;
    if (i == DEC_SIZE * U_SIZE - 1 && buff && get_exp(v_1) && get_exp(v_2)) {
      if (get_exp(v_1) == 1) {
        over_dig = reduce_by_10(&v_1) + reduce_by_10(&v_2);
      } else {
        reduce_by_10(&v_1);
        reduce_by_10(&v_2);
      }
      set_exp(&v_1, get_exp(v_1) - 1);
      set_exp(&v_2, get_exp(v_2) - 1);
      buff = s21_add(v_1, v_2, &tmp);
    }
  }
  if (!buff && over_dig) {
    buff = add(tmp,
               (s21_decimal){
                   {get_bit(tmp, 0) + 2 * (!get_bit(tmp, 0) && over_dig > 10),
                    0, 0, tmp.bits[3]}},
               &tmp);
  }
  if (!buff) {
    cpy(tmp, res, 1);
  } else if (get_sign(v_1)) {
    buff = 2;
  }
  return buff;
}