#include "utils/file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_read_file_size() {
    printf("#########################\n");
    printf("### test_read_file_size()\n");
    printf("#########################\n");

    FILE *f = fopen("../tests/utils/foo.txt", "r");
    size_t size = read_file_size(f);
    printf("File size: %lu\n", size);

    fclose(f);

    if (size != 1274) {
        printf("File size not matching, aborting\n");
        exit(1);
    }

    return 0;
}

int test_read_plaintext_file() {
    printf("#########################\n");
    printf("### test_read_file_size()\n");
    printf("#########################\n");

    size_t size;
    char *buffer = read_plaintext_file("../tests/utils/foo.txt", &size);

    if (strncmp(buffer, "I'd just like to interject for a moment", 39) != 0 ||
        strncmp(buffer + size - 11, "GNU/Linux!", 10) != 0) {
        printf("Head and tail of text not matching, aborting\n");
        exit(1);
    }
    printf("Head and tail test: OK\n");

    return 0;
}

int main(){
    test_read_file_size();
    test_read_plaintext_file();

    return 0;
}
