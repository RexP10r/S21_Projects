#include "../s21_decimal.h"

#define MAX_INT 2147483647
#define MIN_INT -2147483648

#define MIN_FLOAT 1e-28f
#define MAX_FLOAT 79228162514264337593543950335.0

static int get_ld(s21_decimal src, long double *dst) {
  int err = 1;
  if (!validate(src)) {
    long double res = 0.0;
    for (int i = 96; i >= 0; i--) {
      if (get_bit(src, i)) {
        res += powl(2.0L, i);
      }
    }
    int exp = get_exp(src);
    res *= powl(10.0L, -exp);
    if (get_sign(src)) {
      res *= -1;
    }
    *dst = res;
    err = 0;
  }
  return err;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int fl = 1;
  if (dst != NULL) {
    fl = 0;
    zeroize(dst);
    dst->bits[0] = (src < 0) ? (ui_32)(-(long)src) : (ui_32)src;
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
    // ниже проблемный кусок
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

// int s21_from_decimal_to_int(s21_decimal src, int *dst) {
//   int err = 1;
//   long double res = 0.0;

//   if (dst && !get_ld(src, &res) && src.bits[1] == 0 && src.bits[2] == 0 &&
//       res >= MIN_INT && res <= MAX_INT) {
//     *dst = (int)(res);
//     err = 0;
//   }
//   return err;
// }

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int err = 1;
  long double res = 0.0;

  if (dst && !get_ld(src, &res)) {
    long double abs_res = fabsl(res);

    if (abs_res > 0 && abs_res < MIN_FLOAT) {
      *dst = 0.0f;
    } else if (abs_res <= MAX_FLOAT && isfinite(res)) {
      *dst = (float)(roundl(res * 1e7) / 1e7);
      err = 0;
    }
  }
  return err;
}

// static int get_double_sign(double src) {
//   ui_64 bits = 0;
//   memcpy(&bits, &src, sizeof(bits));
//   return (int)((bits & 0x8000000000000000ULL) >> 63);
// }

static int get_double_exp(double src) {
  unsigned long long bits = 0;
  memcpy(&bits, &src, sizeof(bits));
  return (int)((bits & 0x7FF0000000000000ULL) >> 52) - 1023;
}

static unsigned long long get_double_mant(double src) {
  unsigned long long bits = 0;
  memcpy(&bits, &src, sizeof(bits));
  return (bits) & 0xFFFFFFFFFFFFFULL;
}

static int get_float_sign(float src) {
  ui_32 bits = 0;
  memcpy(&bits, &src, sizeof(bits));
  return (int)((bits & 0x80000000ULL) >> 31);
}

// static int get_float_exp(float src) {
//   ui_32 bits = 0;
//   memcpy(&bits, &src, sizeof(bits));
//   return (int)((bits & 0x7F800000ULL) >> 23) - 127;
// }

// static ui_32 get_float_mant(float src) {
//   ui_32 bits = 0;
//   memcpy(&bits, &src, sizeof(bits));
//   return bits & 0x007FFFFFULL;
// }

// void print_double_bin(double src) {
//   unsigned long long bits = 0;
//   memcpy(&bits, &src, sizeof(bits));
//   for (int i = sizeof(double) * 8 - 1; i >= 0; i--) {
//     printf("%llu", (bits >> i) & 1);
//     if (i == 63 || i == 52) printf(" ");
//   }
//   printf("\n");
// }

// void print_float_bin(float src) {
//   unsigned int bits = 0;
//   memcpy(&bits, &src, sizeof(bits));
//   for (int i = sizeof(float) * 8 - 1; i >= 0; i--) {
//     printf("%u", (bits >> i) & 1);
//     if (i == 31 || i == 23) printf(" ");
//   }
//   printf("\n");
// }

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int err = 0;
  if (!dst) {
    err = 1;
  } else if (src == 0.0) {
    zeroize(dst);
    set_sign(dst, get_float_sign(src));
  } else if (isinf(src) || isnan(src) || fabs(src) > MAX_FLOAT) {
    err = 1;
  } else if (fabs(src) < MIN_FLOAT) {
    zeroize(dst);
    err = 1;
  } else {
    int sign = (src < 0) ? 1 : 0;
    double dbl_src = (double)fabsf(src);
    int scale = 0;
    if (dbl_src >= 1e6) {
      while (dbl_src >= 1e7) {
        scale++;
        dbl_src /= 10;
      }
      dbl_src = round(dbl_src);
      dbl_src *= pow(10, scale);
      scale = 0;
    } else {
      while (dbl_src <= 1e7) {
        scale++;
        dbl_src *= 10;
      }
      dbl_src = round(dbl_src);
      while (((int)dbl_src % 10 == 0) && (scale > 0)) {
        dbl_src /= 10;
        scale--;
      }
    }
    unsigned long long dbl_src_mant = get_double_mant(dbl_src);
    int dbl_src_exp = get_double_exp(dbl_src);
    if (dbl_src_exp > 95 || dbl_src_exp < 0) {
      err = 1;
    } else {
      zeroize(dst);
      set_bit(dst, dbl_src_exp);
      for (int i = dbl_src_exp - 1, j = 0; (i >= 0) && (j <= 51); i--, j++) {
        if (dbl_src_mant & (1ULL << (51 - j))) set_bit(dst, i);
      }
      set_sign(dst, sign);
      set_exp(dst, scale);
    }
  }
  return err;
}