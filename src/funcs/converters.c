#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int fl = 1;
  if (dst != NULL) {
    fl = 0;
    zeroize(dst);
    dst->bits[0] = abs(src);
    dst->bits[3] &= ~(1u << 31);
    dst->bits[3] |= (unsigned int)(src < 0) << 31;
  }
  return fl;
}
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int fl = 0;
  if (dst == NULL) fl = 1;
  if (src.bits[1] != 0 || src.bits[2] != 0) {
    fl = 1;
  }
  if (fl == 0) {
    int scale = (src.bits[3] >> 16) & 0xFF;
    int sign = (src.bits[3] >> 31) & 1;
    unsigned int abs_value = src.bits[0];

    for (int i = 0; i < scale; i++) {
      abs_value /= 10;
    }

    if (sign) {
      if (abs_value > 2147483648u) {
        fl = 1;
      }
    } else {
      if (abs_value > 2147483647) {
        fl = 1;
      }
    }
    if (fl == 0) {
      int result = (int)abs_value;
      if (sign) {
        result = -result;
      }

      *dst = result;
    }
  }
  return fl;
}