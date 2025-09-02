#include "../s21_decimal.h"

int normalize(s21_decimal *a, s21_decimal *b) {
  int over_dig = 0;
  if (is_zero(*a) && is_zero(*b)) {
    set_exp(a, 0);
    set_exp(b, 0);
  } else {
    unsigned char exp_1 = get_exp(*a), exp_2 = get_exp(*b);
    if (exp_1 > exp_2) {
      over_dig = move(b, a);
    } else if (exp_1 < exp_2) {
      over_dig = move(a, b);
    }
  }
  return over_dig;
}

int move(s21_decimal *exp_low, s21_decimal *exp_high) {
  unsigned char e_low = get_exp(*exp_low), e_high = get_exp(*exp_high),
                overflow = 0, over_dig = 0;
  while (e_low < e_high) {
    if (!is_zero(*exp_high) && get_exp(*exp_low) < 28 && !overflow &&
        !(overflow = raise_by_10(exp_low))) {
      set_exp(exp_low, ++e_low);
    } else if (!is_zero(*exp_low) && get_exp(*exp_high)) {
      if (get_exp(*exp_high) == 1) {
        over_dig = reduce_by_10(exp_high);
      } else {
        reduce_by_10(exp_high);
      }
      set_exp(exp_high, --e_high);
    }
  }
  return over_dig;
}

int raise_by_10(s21_decimal *v) {
  s21_decimal buff_1 = {0}, buff_2 = {0};
  int i = 3, overflow = 0, sign = get_sign(*v);
  cpy(*v, &buff_1, 1);
  cpy(*v, &buff_2, 1);
  shift_left(&buff_2);
  while (i-- && !(overflow = shift_left(&buff_1))) {
  }
  if (!overflow) {
    overflow = add(buff_1, buff_2, v);
    set_sign(v, sign);
  }
  return overflow;
}

int reduce_by_10(s21_decimal *v) {
  s21_decimal divisor = {{10, 0, 0, v->bits[3]}},
              incr = {{1, 0, 0, v->bits[3]}}, res = {{0, 0, 0, v->bits[3]}},
              rem = {0};
  while (1) {
    while (!(divisor.bits[2] >> 31) &&
           (get_sign(*v) ? s21_is_greater_or_equal(divisor, *v)
                         : s21_is_less_or_equal(divisor, *v))) {
      shift_left(&divisor);
      shift_left(&incr);
    }
    if (incr.bits[0] == 1) {
      break;
    }
    if (get_sign(*v) ? !s21_is_greater(divisor, *v)
                     : !s21_is_less(divisor, *v)) {
      shift_right(&divisor);
      shift_right(&incr);
    }
    add(res, incr, &res);
    sub(*v, divisor, v);
    init_reduce(&divisor, 10, divisor.bits[3]);
    init_reduce(&incr, 1, incr.bits[3]);
  }
  init_reduce(&divisor, 10, divisor.bits[3]);
  div_mod(*v, divisor, &(s21_decimal){0}, &rem);
  cpy(res, v, 0);
  return rem.bits[0];
}

void init_reduce(s21_decimal *v, unsigned int to_set, unsigned int meta) {
  v->bits[0] = to_set;
  v->bits[1] = 0;
  v->bits[2] = 0;
  v->bits[3] = meta;
}