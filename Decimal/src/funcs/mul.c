#include "../s21_decimal.h"
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int result_function = 0;

  if (!is_valid_scale(value_1) || !is_valid_scale(value_2) || !result) {
    result_function = 3;
  } else {
    int sign = (value_1.bits[3] >> 31) ^ (value_2.bits[3] >> 31);
    int scale =
        ((value_1.bits[3] >> 16) & 0xFF) + ((value_2.bits[3] >> 16) & 0xFF);

    s21_decimal temp_decimal = {0};
    ui_64 temp[6] = {0};

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        ui_64 product = (ui_64)(unsigned int)value_1.bits[i] *
                        (unsigned int)value_2.bits[j];
        ui_64 sum = temp[i + j] + product;
        temp[i + j] = sum & 0xFFFFFFFF;
        temp[i + j + 1] += sum >> 32;
      }
    }

    for (int i = 0; i < 3; i++) {
      temp_decimal.bits[i] = temp[i] & 0xFFFFFFFF;
      if (i < 2) {
        temp_decimal.bits[i + 1] |= (temp[i] >> 32) & 0xFFFFFFFF;
      }
    }

    int overflow = normalize_dec(&temp_decimal, &scale);
    if (overflow) {
      result_function = sign ? 2 : 1;
    } else {
      for (int i = 0; i < 3; i++) {
        result->bits[i] = temp_decimal.bits[i];
      }
      result->bits[3] = (scale << 16) | ((unsigned int)sign << 31);
      if (result->bits[0] == 0 && result->bits[1] == 0 &&
          result->bits[2] == 0) {
        result->bits[3] &= ~(1u << 31);
      }
    }
  }
  return result_function;
}
