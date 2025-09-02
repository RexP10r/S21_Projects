#include "s21_grep.h"

int da(int argc, const char *argv[]) {
  int mods[10] = {0};
  int secFileArgStart = 0, firstFileArgStart = 0;
  secFileArgStart = get_mods(mods, argv, argc);
  int exit = 0;
  StringArray patterns = {0};
  regex_t *regex = NULL;
  StringArray *strings = NULL;
  if ((mods[2] && mods[9]) || secFileArgStart == -1) exit = 1;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s [OPTIONS] <patterns> [filename]\n", argv[0]);
    exit = 1;
  }
  if (!exit) {
    int *args = malloc(sizeof(int));
    int *args_idxs = malloc(sizeof(int));
    int len = get_args(argv, &args, &args_idxs, argc, secFileArgStart,
                       &firstFileArgStart);
    if (!len) exit = 1;
    if (!exit) patterns = get_patterns(argv, args, args_idxs, mods, len);
    free(args);
    free(args_idxs);
  }
  if (!exit && patterns.size == 0) {
    fprintf(stderr, "No patterns provided\n");
    freeStringArray(&patterns, 0);
    exit = 1;
  }
  if (!exit) {
    int patterns_num = patterns.size;
    regex = compile_patterns(patterns, mods);
    int *fileArgs = NULL;
    int files_count =
        get_fileArgs(&fileArgs, secFileArgStart, firstFileArgStart, argv, argc);
    strings = get_strings(argv, mods, fileArgs, files_count);
    initializeValStrings(strings, files_count);
    detectStrings(strings, regex, mods, files_count, patterns_num);
    for (int i = 0; i < files_count; i++) {
      if (strings[i].correct)
        addStuff(strings, regex, argv, mods, files_count, patterns_num, i,
                 fileArgs);
    }
    if (!mods[4])
      write_strings(strings, mods, files_count);
    else
      write_matched_files(strings, argv, files_count, fileArgs);  // l flag
    free_all(strings, patterns, regex, files_count, patterns_num);
    free(fileArgs);
  }
  return 0;
}

int get_mod(int mods[], const char a) {
  const char flags[10] = {"eivclnhsfo"};
  int changes = 1;
  for (size_t i = 0; i < 10; i++) {
    if (a == flags[i]) {
      mods[i] = 1;
      changes = 0;
      i = 10;  // break
    }
  }
  return changes;
}

int process_flag_argument(int mods[], const char *arg, int *need_skip_next) {
  size_t len = strlen(arg);
  *need_skip_next = 0;
  int res = 0;

  for (size_t j = 1, breakk = 0; j < (size_t)len && !breakk; j++) {
    char flag_char = arg[j];
    if (get_mod(mods, flag_char)) {
      fprintf(stderr, "grep: invalid option -- '%c'\n", flag_char);
      breakk = 1;
      res = 1;
    }
    if (!breakk && (flag_char == 'e' || flag_char == 'f')) {
      if (j == len - 1) {
        *need_skip_next = 1;
      }
      breakk = 1;
    }
  }
  return res;
}

int get_mods(int mods[], const char *argv[], int argc) {
  int skip_next = 0;
  int flag_found = 0;
  int pattern_found = 0;
  int i = 1;
  int error = 0;
  int result = 0;

  for (int breakk = 0; i < argc && !breakk && !error; i++) {
    if (skip_next) {
      pattern_found = 1;
      skip_next = 0;
      continue;
    }

    if (argv[i][0] == '-' && argv[i][1] != '\0') {
      int need_skip_next = 0;
      flag_found = 1;
      error = process_flag_argument(mods, argv[i], &need_skip_next);

      if (error) {
        continue;
      }
      if (need_skip_next && i + 1 >= argc) {
        fprintf(stderr, "grep: option requires an argument\n");
        error = 1;
        continue;
      } else if (need_skip_next && i + 1 <= argc) {
        skip_next = 1;
      }
    } else if (flag_found &&
               (pattern_found || (!mods[0] && !mods[8] && argv[1][0] != '-'))) {
      breakk = 1;
      i--;
      continue;
    } else if (i > 0 && argv[i - 1][0] == '-') {
      pattern_found = 1;
    }
  }

  if (error) {
    result = -1;
  } else {
    result = (i >= argc) ? 0 : i;
  }
  if (!flag_found && argc == 3) result = 2;

  return result;
}

#define new_args(args, args_idxs, j)                              \
  int *new_args = realloc(args, (j + 1) * sizeof(int));           \
  args = new_args;                                                \
  int *new_args_idxs = realloc(args_idxs, (j + 1) * sizeof(int)); \
  args_idxs = new_args_idxs;

int get_args(const char *argv[], int **args_ptr, int **args_idxs_ptr, int argc,
             int secFileArgStart, int *firstFileArgStart) {
  int j = 0;
  int *args = *args_ptr;
  int *args_idxs = *args_idxs_ptr;

  for (size_t i = 0; i < (size_t)argc; i++) {
    if (argv[i][0] == '-' && argv[i + 1][0] != '-' &&
        i != (size_t)secFileArgStart - 1) {
      new_args(args, args_idxs, j);
      args[j] = i + 1;
      args_idxs[j] = (argv[i][strlen(argv[i]) - 1] == 'f') ? 1 : 0;
      j++;
    }
  }
  if (argv[1][0] != '-' && !j) {
    new_args(args, args_idxs, j);
    args[j] = 1, args_idxs[j] = 0;
    j++;
  } else
    *firstFileArgStart = 1;

  *args_ptr = args;
  *args_idxs_ptr = args_idxs;

  if (j == 0) {
    free(args);
    free(args_idxs);
    *args_ptr = NULL;
    *args_idxs_ptr = NULL;
  }
  return j;
}

StringArray get_patterns(const char *argv[], const int *args,
                         const int *args_idxs, const int *mods, int len) {
  StringArray patterns;
  initStringArray(&patterns, 10);
  patterns.val_strings = NULL;
  for (size_t i = 0; i < (size_t)len; i++) {
    int *val = realloc(patterns.val_strings, sizeof(int) * (patterns.size + 1));
    patterns.val_strings = val;
    if (args_idxs[i]) {
      int old_size = patterns.size;
      if (readFileToStringArray(argv[args[i]], &patterns)) {
        if (!mods[7])
          fprintf(stderr, "Failed to read file with patterns: %s\n",
                  argv[args[i]]);
      }
      int new_size = patterns.size;
      for (size_t k = (size_t)old_size; k < (size_t)new_size; k++)
        patterns.val_strings[k] = 1;
    } else {
      addString(&patterns, argv[args[i]]);
      patterns.val_strings[patterns.size - 1] = 0;
    }
  }
  return patterns;
}

regex_t *compile_patterns(StringArray patterns, const int *mods) {
  regex_t *regex = calloc(patterns.size, sizeof(regex_t));
  for (size_t i = 0; i < (size_t)patterns.size; i++) {
    int ret = 1;
    if (!mods[1] && patterns.val_strings[i])
      ret = regcomp(&regex[i], patterns.data[i], REG_EXTENDED);  // i & e flags
    if (mods[1] && patterns.val_strings[i])
      ret = regcomp(&regex[i], patterns.data[i], REG_EXTENDED | REG_ICASE);
    if (mods[1] && !patterns.val_strings[i])
      ret = regcomp(&regex[i], patterns.data[i], REG_ICASE);
    if (!mods[1] && !patterns.val_strings[i])
      ret = regcomp(&regex[i], patterns.data[i], 0);
    if (ret != 0) {
      char error_msg[100];
      regerror(ret, &regex[i], error_msg, sizeof(error_msg));
      fprintf(stderr, "Compilation error regex: %s\n", error_msg);
      for (size_t j = 0; j < i; j++) regfree(&regex[j]);
      freeStringArray(&patterns, 0);
      regfree(regex);
      regex = NULL;       //
      i = patterns.size;  // return NULL;
    }
  }
  return regex;
}

void getArg(int **fileArgs, int i, int x) {
  int *newArg = (int *)realloc(*fileArgs, sizeof(int) * (i + 2));
  *fileArgs = newArg;
  (*fileArgs)[i] = x;
}

int get_fileArgs(int **fileArgs, int secFileArgStart, int firstFileArgStart,
                 const char *argv[], int argc) {
  int j = 0;
  if (firstFileArgStart)
    for (size_t i = 1; argv[i][0] != '-'; i++) getArg(fileArgs, j, i), j++;

  if (secFileArgStart != argc && secFileArgStart)
    for (size_t i = (size_t)secFileArgStart; i < (size_t)argc; i++)
      getArg(fileArgs, j, i), j++;

  return j;
}

StringArray *get_strings(const char *argv[], const int *mods,
                         const int *fileArgs, int files_count) {
  StringArray *strings = NULL;
  if (files_count > 0) {
    strings = calloc(files_count, sizeof(StringArray));
    for (size_t i = 0; i < (size_t)files_count; i++) {
      initStringArray(&strings[i], 10);
      if (readFileToStringArray(argv[fileArgs[i]], &strings[i]) != 0) {
        if (!mods[7])
          fprintf(stderr, "Failed to read: %s\n", argv[fileArgs[i]]);
      }
    }
  }
  return strings;
}

void free_all(StringArray *arr1, StringArray arr2, regex_t *regex,
              int files_count, int patterns_num) {
  freeStringArray(&arr2, 1);
  for (size_t i = 0; i < (size_t)files_count; i++) freeStringArray(&arr1[i], 1);
  free(arr1);
  for (size_t i = 0; i < (size_t)patterns_num; i++) regfree(&regex[i]);
  free(regex);
}
