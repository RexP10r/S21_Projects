#include "../s21_decimal.h"

void div_cycle(s21_decimal a, s21_decimal b, s21_decimal *result,
               int *exp_res) {
  s21_decimal r = {0}, q = {0};
  int breakk = 0;
  div_mod(a, b, &q, &r);
  *result = q;
  while (!breakk && !is_zero(r) && *exp_res < 28) {
    breakk = mul_10_64bit(result) || mul_10_64bit(&r);
    div_mod(r, b, &q, &r);
    breakk = breakk || add(*result, q, result);
    (*exp_res)++;
  }
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (is_zero(value_2)) return 3;

  int exp_a = get_exp(value_1);
  int exp_b = get_exp(value_2);

  int sign_a = get_sign(value_1);
  int sign_b = get_sign(value_2);

  int exp_res = exp_a - exp_b;

  value_1.bits[3] = 0x00000000;
  value_2.bits[3] = 0x00000000;

  div_cycle(value_1, value_2, result, &exp_res);

  int sign = sign_a ^ sign_b;
  int res = (exp_res >= 0) ? 0 : ((!sign) ? 1 : 2);

  if (!res) {
    set_sign(result, sign);
    if (!is_zero(*result)) set_exp(result, exp_res);
  } else {
    zeroize(result);
  }

  return res;
}