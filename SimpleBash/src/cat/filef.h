#define FILEF
#ifdef FILEF

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_mods(int mods[], const char *argv[], int argc);
int v_flag(int c, const int *mods);
int process_file(FILE *file, const int *mods, const int last_num,
                 int last_char);
void main_cycle(const char *argv[], const int *mods, int argc,
                int fileArgStart);

#endif