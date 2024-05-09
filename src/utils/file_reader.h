/*
 * Functions for work with files
 */

#include <stdlib.h>
#include <stdio.h>

size_t read_file_size(FILE * f);
char *read_plaintext_file(const char *filename, size_t *out_size);
