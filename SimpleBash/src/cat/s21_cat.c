#include "s21_cat.h"

#include "filef.h"

int da(int argc, const char *argv[]) {
  int exit_code = 0;

  if (argc >= 2) {
    int mods[6] = {0};
    int fileArgStart = get_mods(mods, argv, argc);

    if (fileArgStart != -1) {
      main_cycle(argv, mods, argc, fileArgStart);
    } else {
      fprintf(stderr, "Wrong flag\n");
      exit_code = 1;
    }
  } else {
    fprintf(stderr, "Usage: %s [OPTIONS] <filename1> [filename2 ...]\n",
            argv[0]);
    exit_code = 1;
  }

  return exit_code;
}