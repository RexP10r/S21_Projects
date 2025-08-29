#ifndef DECIMAL
#define DECIMAL

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define U_SIZE (int)(sizeof(unsigned int) * 8)
#define DEC_SIZE (int)(sizeof(s21_decimal) / sizeof(unsigned int) - 1)

typedef unsigned long ui_64;
typedef unsigned int ui_32;

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef enum s21_decimal_sign {
  S21_POSITIVE = 0,
  S21_NEGATIVE = 1
} s21_decimal_sign;

// utils
unsigned int get_exp(s21_decimal v);
void set_exp(s21_decimal *v, const unsigned int exp);

int shift_left(s21_decimal *a);
void shift_right(s21_decimal *a);

int is_zero(s21_decimal d);
void zeroize(s21_decimal *);

int get_sign(s21_decimal d);
void set_sign(s21_decimal *d, int sign);
int set_bit(s21_decimal *dst, int pos);
int get_bit(s21_decimal src, int pos);

int is_mantissa_equal(s21_decimal value_1, s21_decimal value_2);
int normalize(s21_decimal *, s21_decimal *);
int move(s21_decimal *exp_low, s21_decimal *exp_high);

int raise_by_10(s21_decimal *);
int reduce_by_10(s21_decimal *);
void init_reduce(s21_decimal *v, unsigned int to_set, unsigned int meta);
int mul_10_64bit(s21_decimal *a);

void cpy(s21_decimal src, s21_decimal *dst, int is_full);

int div_mod(s21_decimal a, s21_decimal b, s21_decimal *q, s21_decimal *r);
void uint96_sub(s21_decimal *a, s21_decimal b);
int compare_mantisses(s21_decimal a, s21_decimal b);
int is_mantissa_equal(s21_decimal value_1, s21_decimal value_2);
int normalize_decimals(s21_decimal *a, s21_decimal *b);
void div_cycle(s21_decimal a, s21_decimal b, s21_decimal *result, int *exp_res);

int s21_banking_round(s21_decimal value, s21_decimal *result);
s21_decimal delete_zeros(s21_decimal value);
int is_valid_scale(s21_decimal value);
int normalize_dec(s21_decimal *a, int *scale);
int is_max(s21_decimal, s21_decimal);

int add(s21_decimal v_1, s21_decimal v_2, s21_decimal *res);
void sub(s21_decimal v_1, s21_decimal v_2, s21_decimal *res);

// main funcs
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);

int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);

int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);

int s21_negate(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);

#endif