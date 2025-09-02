#include "../s21_decimal.h"

unsigned int get_exp(s21_decimal v) { return (v.bits[3] & 0x0FFFFFFF) >> 16; }

void set_exp(s21_decimal *v, const unsigned int exp) {
  if (exp <= 28) {
    v->bits[3] = (v->bits[3] & 0x80000000) | exp << 16;
  }
}

int shift_left(s21_decimal *v) {
  unsigned int msb = 0, lsb = 0;
  for (int i = 0; i < DEC_SIZE; i++, lsb = msb) {
    msb = v->bits[i] >> (U_SIZE - 1);
    v->bits[i] <<= 1;
    v->bits[i] |= lsb;
  }
  return msb;
}

void shift_right(s21_decimal *v) {
  unsigned int lsb = 0, msb = 0;
  for (int i = DEC_SIZE; i--; lsb = msb) {
    msb = v->bits[i] & 1;
    v->bits[i] >>= 1;
    v->bits[i] |= lsb << (U_SIZE - 1);
  }
}

void zeroize(s21_decimal *v) {
  for (int i = DEC_SIZE + 1; i--; v->bits[i] = 0) {
  }
}

int is_zero(s21_decimal d) { return !d.bits[0] && !d.bits[1] && !d.bits[2]; }

void set_sign(s21_decimal *d, int sign) {
  if (sign) {
    d->bits[3] |= 0x80000000;
  } else {
    d->bits[3] &= ~0x80000000;
  }
}

int get_sign(s21_decimal d) { return d.bits[3] >> 31 & 1; }

void cpy(s21_decimal src, s21_decimal *dst, int is_full) {
  for (int i = DEC_SIZE + is_full; i--; dst->bits[i] = src.bits[i]) {
  }
}

int normalize_cycle(s21_decimal *a, ui_32 exp, ui_32 target_exp) {
  int err = 0;
  while (exp < target_exp) {
    ui_64 carry = 0;
    for (int i = 0; i < 3; i++) {
      ui_64 temp = (ui_64)a->bits[i] * 10 + carry;
      a->bits[i] = (ui_32)temp;
      carry = temp >> 32;
    }
    if (carry) {
      err = 1;
      break;
    }
    exp++;
  }
  return err;
}

int mul_10_64bit(s21_decimal *a) {
  ui_64 carry = 0;
  for (int i = 0; i < 3; i++) {
    ui_64 temp = (ui_64)a->bits[i] * 10 + carry;
    a->bits[i] = (ui_32)temp;
    carry = temp >> 32;
  }
  return (carry) ? 1 : 0;
}

int normalize_decimals(s21_decimal *a, s21_decimal *b) {
  ui_32 exp_a = get_exp(*a);
  ui_32 exp_b = get_exp(*b);
  ui_32 target_exp = (exp_a > exp_b) ? exp_a : exp_b;
  int fl = 0;
  if (normalize_cycle(a, exp_a, target_exp))
    fl = 1;
  else {
    if (normalize_cycle(b, exp_b, target_exp))
      fl = 1;
    else {
      set_exp(a, target_exp);
      set_exp(b, target_exp);
    }
  }
  return fl;
}

int div_mod(s21_decimal a, s21_decimal b, s21_decimal *q, s21_decimal *r) {
  s21_decimal remainder = a;
  s21_decimal divisor = b;
  s21_decimal quotient = (s21_decimal){{0, 0, 0, 0}};

  int shift = 0;
  while ((compare_mantisses(divisor, remainder) < 0) &&
         !(divisor.bits[2] & 0x80000000)) {
    shift_left(&divisor);
    shift++;
  }

  for (int i = 0; i <= shift; i++) {
    shift_left(&quotient);
    if (compare_mantisses(divisor, remainder) <= 0) {
      uint96_sub(&remainder, divisor);
      quotient.bits[0] |= 1;
    }
    shift_right(&divisor);
  }

  *q = quotient;
  *r = remainder;
  return 0;
}

int compare_mantisses(s21_decimal a, s21_decimal b) {
  int sign_a = get_sign(a);
  int sign_b = get_sign(b);
  int fl = 0;
  if (sign_a && !sign_b)
    fl = -1;
  else if (!sign_a && sign_b)
    fl = 1;
  else {
    for (int i = 2; i >= 0; i--) {
      if (a.bits[i] < b.bits[i]) fl = -1;
      if (a.bits[i] > b.bits[i]) fl = 1;
    }
  }
  return fl;
}

void uint96_sub(s21_decimal *a, s21_decimal b) {
  ui_64 borrow = 0;
  for (int i = 0; i < 3; i++) {
    ui_64 temp = (ui_64)a->bits[i] - b.bits[i] - borrow;
    borrow = (temp >> 63) & 1;
    a->bits[i] = (ui_32)temp;
  }
}

int is_mantissa_equal(s21_decimal value_1, s21_decimal value_2) {
  int i = 2;
  while (value_1.bits[i] == value_2.bits[i] && i--) {
  }
  return i < 0;
}

int set_bit(s21_decimal *dst, int pos) {
  int res = 1;
  if (dst && pos >= 0 && pos < 128) {
    int n_bits = pos / 32;
    dst->bits[n_bits] |= ((ui_32)1 << (pos - (32 * n_bits)));
    res = 0;
  }
  return res;
}

int get_bit(s21_decimal src, int pos) {
  int res = -1;
  if (pos >= 0 && pos <= 96) {
    res = (src.bits[pos / 32] >> (pos % 32)) & 1;
  }
  return res;
}

s21_decimal delete_zeros(s21_decimal value) {
  s21_decimal result = value;
  int scale = get_exp(value);
  int newscale = scale;
  if (scale == 0) {
    return result;
  }
  int sign = get_sign(value);
  s21_decimal divisor = {{10, 0, 0, 0}};
  s21_decimal remainder = {0};
  s21_decimal tmp = value;
  set_exp(&tmp, 0);
  set_sign(&tmp, 0);
  for (int i = 0; i < scale; ++i) {
    s21_decimal tmp2 = {0};
    div_mod(tmp, divisor, &tmp2, &remainder);
    if (remainder.bits[0] != 0 || remainder.bits[1] != 0 ||
        remainder.bits[2] != 0) {
      break;
    } else {
      tmp = tmp2;
      --newscale;
    }
  }
  set_exp(&tmp, newscale);
  set_sign(&tmp, sign);
  result = tmp;

  return result;
}

int is_valid_scale(s21_decimal value) {
  int scale = (value.bits[3] >> 16) & 0xFF;
  return scale >= 0 && scale <= 28;
}

int normalize_dec(s21_decimal *a, int *scale) {
  int err = 0;
  while (*scale > 28) {
    ui_64 remainder = 0;
    for (int i = 2; i >= 0; i--) {
      ui_64 value = ((ui_64)remainder << 32) | (ui_64)a->bits[i];
      a->bits[i] = value / 10;
      remainder = value % 10;
    }
    (*scale)--;

    // Если есть остаток, возвращаем ошибку
    if (remainder != 0) {
      err = 1;
      break;
    }
  }
  return err;
}

int is_max(s21_decimal v_1, s21_decimal v_2) {
  return is_mantissa_equal(
             v_1, (s21_decimal){{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}) &&
         !get_exp(v_1) && !is_zero(v_2);
}

int validate(s21_decimal src) {
  int res = 1;
  if (!(src.bits[3] & 0x7F000000)) {
    int exp = get_exp(src);
    res = 0;
    if (exp > 28) {
      res = 1;
    }
  }
  return res;
}