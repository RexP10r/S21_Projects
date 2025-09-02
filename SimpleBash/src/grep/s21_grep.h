#define SGREP
#ifdef SGREP

#include "filef.h"

int get_mods(int mods[], const char *argv[], int argc);
int get_args(const char *argv[], int **args, int **args_idxs, int argc,
             int secFileArgStart, int *firstFileArgStart);
StringArray get_patterns(const char *argv[], const int *args,
                         const int *args_idxs, const int *mods, int len);
regex_t *compile_patterns(StringArray patterns, const int *mods);
int get_fileArgs(int **fileArgs, int secFileArgStart, int firstFileArgStart,
                 const char *argv[], int argc);
StringArray *get_strings(const char *argv[], const int *mods,
                         const int *fileArgs, int files_count);
void free_all(StringArray *arr1, StringArray arr2, regex_t *regex,
              int files_count, int patterns_num);
int da(int argc, const char *argv[]);

#endif