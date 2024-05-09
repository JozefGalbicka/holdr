
#include "utils/strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_trim()
{
    printf("#########################\n");
    printf("### test_trim()\n");
    printf("#########################\n");
    char str[50] = "   Thisisteststring    ";

    printf("Before trim:\n");
    printf("'%s'\n", str);

    char *new_str = trim(str);
    printf("After trim:\n");
    printf("'%s'\n", new_str);

    if (new_str[0] == ' ' || new_str[strlen(new_str) - 1] == ' ') {
        printf("ERROR OCCURRED\n");
        exit(1);
    }

    return 0;
}

int test_split_str() {
    printf("#########################\n");
    printf("### test_split_str()\n");
    printf("#########################\n");

    char str[] = "This.is\ntest-string\n\n";
    char delimeter[] = ".-\n";
    int split_count;

    char ** split = split_str(str, delimeter, &split_count);

    if (split_count != 4) {
        printf("Split count does not match, aborting\n");
        exit(1);
    }
    printf("Split count test: OK\n");

    if (strcmp(split[0], "This") != 0 || strcmp(split[split_count - 1], "string") != 0) {
        printf("Split strings do not match, aborting\n");
        exit(1);
    }
    printf("Split string begin and end test: OK\n");

    free(split);

    return 0;
}

int main(void)
{
    test_trim();
    test_split_str();

    return 0;
}
