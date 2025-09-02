#include "filef.h"

int process_long_option(const char *opt, int *mods) {
  if (!strcmp(opt, "number-nonblank")) {
    mods[4] = 1;
    return 0;
  }
  if (!strcmp(opt, "number")) {
    mods[3] = 1;
    return 0;
  }
  if (!strcmp(opt, "squeeze-blank")) {
    mods[0] = 1;
    return 0;
  }
  if (!strcmp(opt, "show-tabs")) {
    mods[1] = 1;
    return 0;
  }
  if (!strcmp(opt, "show-ends")) {
    mods[2] = 1;
    return 0;
  }
  return -1;
}

int process_short_option(char c, int *mods) {
  switch (c) {
    case 's':
      mods[0] = 1;
      return 0;
    case 't':
      mods[1] = mods[5] = 1;
      return 0;
    case 'e':
      mods[2] = mods[5] = 1;
      return 0;
    case 'T':
      mods[1] = 1;
      return 0;
    case 'E':
      mods[2] = 1;
      return 0;
    case 'n':
      mods[3] = 1;
      return 0;
    case 'b':
      mods[4] = 1;
      return 0;
    case 'v':
      mods[5] = 1;
      return 0;
    default:
      return -1;
  }
}

int get_mods(int *mods, const char *argv[], int argc) {
  for (size_t i = 1; i < (size_t)argc; i++) {
    if (argv[i][0] != '-') return i;
    if (argv[i][1] == '-') {
      const char *opt = argv[i] + 2;
      if (process_long_option(opt, mods) != 0) return -1;
    } else {
      for (size_t j = 1; argv[i][j] != '\0'; j++) {
        if (process_short_option(argv[i][j], mods) != 0) return -1;
      }
    }
  }
  return argc;
}

void main_cycle(const char *argv[], const int *mods, int argc,
                int fileArgStart) {
  int last_num = 1, last_char = '\n';
  for (size_t i = fileArgStart; i < (size_t)argc; i++) {
    FILE *file = fopen(argv[i], "r");
    int fsize = 0;
    if (file) {
      fseek(file, 0, SEEK_END);
      fsize = ftell(file);
      rewind(file);
    }
    if (file && fsize) {
      last_num = process_file(file, mods, last_num, last_char);
    } else {
      fprintf(stderr, "Failed to read file: %s\n", argv[i]);
    }
  }
}

int v_flag(int c, const int *mods) {
  int res = 0, exit = 0;
  if (c == '\t' && mods[1]) {
    printf("^I");
    res = 1;
    exit = 1;
  }

  if (mods[5] && c != '\n' && !exit) {  // v flag
    if (c < 32) {
      if (c == '\t')
        printf("\t");
      else
        printf("^%c", c + 64);
      res = 1;
    } else if (c == 127) {
      printf("^?");
      res = 1;
    }
  }
  return res;
}

int process_file(FILE *file, const int *mods, const int last_num,
                 int last_char) {
  int current_char;
  int printed_blank = 0;
  int line_num = last_num;

  while ((current_char = fgetc(file)) != EOF) {
    if (mods[0] && current_char == '\n' && last_char == '\n' &&
        printed_blank) {  // s flag
      last_char = current_char;
      continue;
    }

    if (last_char == '\n' &&
        ((mods[4] && current_char != '\n') || (mods[3] && !mods[4]))) {
      printf("%6d\t", line_num++);
    }

    if (current_char == '\n') {
      if (mods[2]) printf("$");
      putchar('\n');
      if (last_char == '\n') {
        printed_blank = 1;
      } else {
        printed_blank = 0;
      }
    } else {
      if (v_flag(current_char, mods)) {
        last_char = current_char;
        continue;
      }
      if (current_char == '\t' && mods[1])
        printf("\t");
      else
        putchar(current_char);

      printed_blank = 0;
    }
    last_char = current_char;
  }
  return line_num;
}