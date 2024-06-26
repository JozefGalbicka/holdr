
#ifndef STRINGS_H
#define STRINGS_H

/*
 * Helpful functions to alter string(s)
 */

#include <stdlib.h>

char *right_trim(char *str);
char *left_trim(char *str);
char *trim(char *str);
char *get_basename(const char *path);
char **split_str(const char *str, char *delimeter, int *split_count_out);
void reverse_string(char *str, int start, int end);
void reverse_domain(char *domain);
void strip_trailing_dot(char *domain);

#endif
