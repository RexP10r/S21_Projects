#include "filef.h"

char *my_strdup(const char *str) {
  char *copy = NULL;  //
  if (str) {
    size_t len = strlen(str) + 1;
    copy = malloc(len);
    if (copy) memcpy(copy, str, len);
  }
  return copy;
}

void initStringArray(StringArray *arr, int initialCapacity) {
  arr->data = malloc(initialCapacity * sizeof(char *));
  arr->size = 0;
  arr->capacity = initialCapacity;
  arr->val_strings = NULL;
}

void addString(StringArray *arr, const char *str) {
  if (arr->size >= arr->capacity) {
    size_t newCapacity =
        arr->size == arr->capacity - 1 ? 4 : arr->capacity + 10;
    char **newData = realloc(arr->data, newCapacity * sizeof(char *));
    arr->data = newData;
    arr->capacity = newCapacity;
  }

  arr->data[arr->size++] = my_strdup(str);
}

void initMatchArray(MatchArray *arr, int initial_size) {
  arr->matches = (Match *)malloc(initial_size * sizeof(Match));
  arr->size = 0;
  arr->capacity = initial_size;
}

void addMatch(MatchArray *arr, Match m) {
  if (arr->size >= arr->capacity) {
    arr->capacity += 10;
    arr->matches =
        (Match *)realloc(arr->matches, arr->capacity * sizeof(Match));
  }
  arr->matches[arr->size++] = m;
}

void freeMatchArray(MatchArray *arr) {
  for (size_t i = 0; i < (size_t)arr->size; i++) {
    free(arr->matches[i].pattern);
  }
  free(arr->matches);
}

void freeStringArray(StringArray *arr, int free_val) {
  for (size_t i = 0; i < (size_t)arr->size; i++) {
    free(arr->data[i]);
  }
  free(arr->data);
  if (free_val) free(arr->val_strings);
  arr->size = 0;
}

int readFileToStringArray(const char *filename, StringArray *arr) {
  FILE *file = fopen(filename, "r");
  int res = 0;

  if (file) {  //
    fseek(file, 0, SEEK_END);
    int fsize = ftell(file);
    rewind(file);
    if (fsize) {
      arr->correct = 1;
      char buffer[1024];
      while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        addString(arr, buffer);
      }
    } else {
      arr->correct = 1;
      addString(arr, "\0");
    }
    fclose(file);
  } else {
    arr->correct = 0;
    res = 1;
  }
  return res;
}

int count_val(StringArray *arr) {
  int count = 0;
  for (size_t i = 0; i < (size_t)arr->size; i++) {
    count += arr->val_strings[i];
  }
  return count;
}

int count_digits(int num) {
  char buffer[20];
  sprintf(buffer, "%d", abs(num));
  return strlen(buffer);
}

int count_matches(regex_t *preg, const char *string) {
  int count = 0;
  size_t offset = 0;
  const size_t max_matches = 1;
  regmatch_t pmatch[max_matches];

  int stop = 0;
  int error = 0;

  while (!stop) {
    int ret = regexec(preg, string + offset, max_matches, pmatch, 0);

    if (ret != 0) {
      if (ret == REG_NOMATCH) {
        stop = 1;
      } else {
        char error_msg[100];
        regerror(ret, preg, error_msg, sizeof(error_msg));
        fprintf(stderr, "Error regex: %s\n", error_msg);
        error = 1;
        stop = 1;
      }
    } else {
      count++;
      offset += pmatch[0].rm_eo;

      if (string[offset] == '\0') {
        stop = 1;
      }
    }
  }

  return error ? -1 : count;
}

void calculate_matches(regex_t *regex, int num_patterns, const char *orig_str,
                       StringArray *arr, int i, int j, int *true_val_count) {
  int count, max_count = 0;
  for (size_t k = 0; k < (size_t)num_patterns; k++) {
    count = count_matches(&regex[k], orig_str);
    if (count > max_count) {
      max_count = count;
    }
  }
  count = 0;
  for (size_t k = 0; k < (size_t)num_patterns; k++)
    count += count_matches(&regex[k], orig_str);
  *true_val_count = (count) ? count_val(&arr[i]) : 0;
  arr[i].val_strings[j] = (arr[i].val_strings[j] == -1) ? 1 : max_count;
}

char *format_line(const char *prefix, const char *orig_str, int line_num,
                  int true_val_count, int flag_n, int flag_c) {
  size_t needed_size;
  int line_num_len = count_digits(line_num);

  if (prefix) {
    needed_size = strlen(prefix) + strlen(orig_str) + 6;
    if (flag_n && flag_c) needed_size += line_num_len + 3;
    if (flag_c) needed_size += count_digits(true_val_count) - strlen(orig_str);
  } else {
    needed_size = strlen(orig_str) + 6;
    if (flag_n && flag_c) needed_size += line_num_len + 3;
    if (flag_c) needed_size += count_digits(true_val_count) - strlen(orig_str);
  }

  char *buf = malloc(needed_size);
  if (prefix) {
    if (flag_c) {
      snprintf(buf, needed_size, "%s:%d", prefix, true_val_count);
    } else if (flag_n) {
      snprintf(buf, needed_size, "%s:%d:%s", prefix, line_num, orig_str);
    } else {
      snprintf(buf, needed_size, "%s:%s", prefix, orig_str);
    }
  } else {
    if (flag_c) {
      snprintf(buf, needed_size, "%d", true_val_count);
    } else if (flag_n) {
      snprintf(buf, needed_size, "%d:%s", line_num, orig_str);
    } else {
      snprintf(buf, needed_size, "%s", orig_str);
    }
  }
  return buf;
}

void collect_matches(const char *arr, int line_num, regex_t *regex,
                     MatchArray *match_arr) {
  size_t offset = 0;
  regmatch_t pmatch[1];
  int stop = 0;

  while (!stop) {
    int ret = regexec(regex, arr + offset, 1, pmatch, 0);
    if (ret == REG_NOMATCH) {
      stop = 1;
    } else if (ret != 0) {
      char error_msg[100];
      regerror(ret, regex, error_msg, sizeof(error_msg));
      fprintf(stderr, "Error regex: %s\n", error_msg);
      stop = 1;
    } else {
      int start = offset + pmatch[0].rm_so;
      int end = pmatch[0].rm_eo;
      int len = end - pmatch[0].rm_so;

      char *patt = (char *)malloc(len + 1);
      strncpy(patt, arr + offset + pmatch[0].rm_so, len);
      patt[len] = '\0';

      Match m = {start, patt, line_num};
      addMatch(match_arr, m);
      offset += end;

      if (arr[offset] == '\0') stop = 1;
    }
  }
}

int compare_matches(const void *a, const void *b) {
  const Match *match_a = (const Match *)a;
  const Match *match_b = (const Match *)b;
  return match_a->start - match_b->start;
}

void transform_string_array(StringArray *arr, regex_t *regex, int regex_count) {
  StringArray result = {0};
  initStringArray(&result, 10);
  result.val_strings = NULL;

  for (size_t i = 0; i < (size_t)arr->size; i++) {
    MatchArray match_arr;
    initMatchArray(&match_arr, 10);

    for (size_t r = 0; r < (size_t)regex_count; r++) {
      collect_matches(arr->data[i], i + 1, &regex[r], &match_arr);
    }
    qsort(match_arr.matches, match_arr.size, sizeof(Match), compare_matches);

    for (size_t j = 0; j < (size_t)match_arr.size; j++) {
      int *new_val =
          (int *)realloc(result.val_strings, sizeof(int) * (result.size + 1));
      result.val_strings = new_val;
      result.val_strings[result.size] = match_arr.matches[j].line_num;
      addString(&result, match_arr.matches[j].pattern);
    }

    freeMatchArray(&match_arr);
  }

  freeStringArray(arr, 1);
  if (result.size) result.correct = arr->correct;
  *arr = result;
}

void addStuff(StringArray *arr, regex_t *regex, const char *argv[],
              const int *mods, int num_files, int num_patterns, int i,
              const int *fileArgs) {
  const int flag_v = mods[2];
  const int flag_c = mods[3];
  const int flag_n = mods[5];
  const int flag_o = mods[9];
  const int multi_file = (num_files > 1 && !mods[6]) ? 1 : 0;

  char *prefix = multi_file ? my_strdup(argv[fileArgs[i]]) : NULL;

  if (flag_o && !flag_c) {
    transform_string_array(&arr[i], regex, num_patterns);
  }

  const int file_size = arr[i].size;
  char **new_data = malloc(arr[i].size * sizeof(char *));

  for (size_t j = 0; j < (size_t)file_size; j++) {
    new_data[j] = NULL;
    const char *orig_str = arr[i].data[j];
    const size_t line_num =
        flag_o && !flag_c ? (size_t)arr[i].val_strings[j] : (j + 1);
    int true_val_count = 0;

    if (flag_c) {
      calculate_matches(regex, num_patterns, orig_str, arr, i, j,
                        &true_val_count);
    }

    if (flag_v) true_val_count = file_size - true_val_count;

    new_data[j] = format_line(prefix, orig_str, (int)line_num, true_val_count,
                              flag_n, flag_c);
    if (flag_o && !flag_c && arr->size) arr[i].val_strings[j] = 1;
  }

  if (prefix) free(prefix);

  for (size_t j = 0; j < (size_t)arr[i].size; j++) free(arr[i].data[j]);
  free(arr[i].data);

  arr[i].data = new_data;
}

void initializeValStrings(StringArray *arr, int num_files) {
  for (size_t i = 0; i < (size_t)num_files; i++) {
    arr[i].val_strings = (int *)calloc(arr[i].size, sizeof(int));
  }
}

void detectStrings(StringArray *arr, regex_t *regex, const int *mods,
                   int num_files, int num_patterns) {
  for (size_t i = 0; i < (size_t)num_files; i++) {
    if (!arr->correct) continue;
    for (size_t j = 0; j < (size_t)arr[i].size; j++) {
      int match_found = 0;
      for (size_t k = 0; k < (size_t)num_patterns; k++) {
        int ret = regexec(&regex[k], arr[i].data[j], 0, NULL, 0);
        match_found = !ret ? 1 : match_found;
      }
      arr[i].val_strings[j] =
          (mods[2] && !mods[3]) ? !match_found : match_found;  // v flag
    }
    if (mods[3] && !count_val(&arr[i]))
      arr[i].val_strings[0] = -1;  // c flag if 0 matches
  }
}

void write_strings(const StringArray *arr, const int *mods, int num_files) {
  for (size_t i = 0; i < (size_t)num_files; i++) {
    int num_writes = 0;
    if (!arr->correct) continue;
    for (size_t j = 0; j < (size_t)arr[i].size; j++) {
      for (size_t k = 0; k < (size_t)arr[i].val_strings[j]; k++) {
        if ((mods[3] && num_writes < 1) || !mods[3])
          printf("%s\n", arr[i].data[j]);  // c flag
        num_writes++;
      }
    }
  }
}

void write_matched_files(StringArray *arr, const char *argv[], int num_files,
                         const int *fileArgs) {
  for (size_t i = 0; i < (size_t)num_files; i++) {
    if (count_val(&arr[i])) printf("%s\n", argv[fileArgs[i]]);
  }
}
