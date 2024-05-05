
/*
 * Helpful functions to alter string(s)
 */

#include <stdlib.h>

char *right_trim(char *str);
char *left_trim(char *str);
char *trim(char *str);
char *get_basename(const char *path);
char ** split_str(const char * str, char * delimeter, int * split_count_out);
