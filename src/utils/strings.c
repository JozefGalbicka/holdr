
#include "strings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
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

char **split_str(const char *str, char *delimeter, int *split_count_out)
{
    size_t str_len = strlen(str) + 1; // include \0 terminator

    char str_copy[str_len];
    strncpy(str_copy, str, str_len);

    char *tok = strtok(str_copy, delimeter);
    int split_count = 0;

    while (tok != NULL) {
        split_count++;
        tok = strtok(NULL, delimeter);
    }

    strncpy(str_copy, str, str_len);
    tok = strtok(str_copy, delimeter);
    char **buffer = calloc(split_count, sizeof(char *));

    for (int i = 0; i < split_count; i++) {
        buffer[i] = calloc(strlen(tok) + 1, sizeof(char));
        strcpy(buffer[i], tok);
        tok = strtok(NULL, delimeter);
    }

    *split_count_out = split_count;
    return buffer;
}

void reverse_string(char *str, int start, int end)
{
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void reverse_domain(char *domain)
{
    int n = strlen(domain);

    // Reverse the entire string
    reverse_string(domain, 0, n - 1);

    // Reverse each part of the domain
    int start = 0;
    for (int i = 0; i <= n; i++) {
        if (domain[i] == '.' || domain[i] == '\0') {
            reverse_string(domain, start, i - 1);
            start = i + 1;
        }
    }
}
void strip_trailing_dot(char *domain)
{
    char *last = strrchr(domain, '.');
    if (last) {
        (*last) = '\0';
    }
}
