#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t read_file_size(FILE * f) {
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    return size;
}

// function returns initialized buffer, don't forget to free
char * read_plaintext_file(const char *filename, size_t *out_size) {
    FILE * f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Could not read file %s\n", filename);
        return NULL;
    }

    int file_size = read_file_size(f);
    char * buffer = calloc(file_size, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Could not allocate memory\n");
        fclose(f);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, file_size, f);
    if (bytes_read != file_size) {
        fprintf(stderr, "Could not read all data from file\n");
        free(buffer);
        fclose(f);
        return NULL;
    }

    fclose(f);
    *out_size = file_size;
    return buffer;
}
