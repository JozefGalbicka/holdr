
#include "strings.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char *right_trim(char *str)
{
    if (*str == '\0')
        return str;
    char *end = str + strlen(str) - 1;
    while (isspace(*end)) {
        --end;
    }
    ++end;
    *end = '\0';
    return str;
}

char *left_trim(char *str)
{
    // if (*str == '\0') return str;
    char *start = str;
    char *start2 = str;
    while (*start2 != '\0' && isspace(*start2)) {
        ++start2;
    }
    if (*start2 == '\0') {
        *start = '\0';
        return start;
    }
    while (*start2 != '\0') {
        *start = *start2;
        ++start;
        ++start2;
    }
    *start = '\0';
    return str;
}

char *trim(char *str)
{
    return left_trim(right_trim(str));
}

char *get_basename(const char *path)
{
    char *basename = strrchr(path, '/');
    if (!basename)
        return strdup(path);
    else
        return strdup(basename + 1);
}
