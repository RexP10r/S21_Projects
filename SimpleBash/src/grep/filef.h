#define FILEF
#ifdef FILEF

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char** data;
  int* val_strings;
  // если в структуре паттерны то это идентификатор регулярного выражения или
  // паттерна если в ней файлы, то это идентификатор тех строк, что нужно
  // писать
  int size;
  int capacity;
  int correct;  // 0 для ложных файлов
} StringArray;

typedef struct {
  int start;
  char* pattern;
  int line_num;
} Match;

typedef struct {
  Match* matches;
  int size;
  int capacity;
} MatchArray;

void initStringArray(StringArray* arr, int initialCapacity);
void addString(StringArray* arr, const char* str);
void freeStringArray(StringArray* arr, int free_val);
int readFileToStringArray(const char* filename, StringArray* arr);
void write_strings(const StringArray* arr, const int* mods, int num_files);
void write_matched_files(StringArray* arr, const char* argv[], int num_files,
                         const int* fileArgs);
void initializeValStrings(StringArray* arr, int num_files);
void detectStrings(StringArray* arr, regex_t* regex, const int* mods,
                   int num_files, int num_patterns);
int count_val(StringArray* arr);
void addStuff(StringArray* arr, regex_t* regex, const char* argv[],
              const int* mods, int num_files, int num_patterns, int i,
              const int* fileArgs);

#endif
