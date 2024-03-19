
#include "utils/strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_trim() {
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

int main(void) {
    test_trim();
    return 0;
}
