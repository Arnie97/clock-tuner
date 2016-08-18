#include "util.h"


void *
alloc(size_t sz)
{
    void *buf = malloc(sz);
    if (!buf) {
        puts("Failed to allocate heap memory!");
        exit(EXIT_FAILURE);
    }
    return buf;
}


FILE *
open(const char *file_name, const char *mode)
{
    FILE *fp = fopen(file_name, mode);
    if (!fp) {
        printf("Failed to open file %s!\n", file_name);
        exit(EXIT_FAILURE);
    }
    return fp;
}


size_t
read_file(char **ptr, const char *file_name)
{
    FILE *fp = open(file_name, "r");
    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    *ptr = alloc(size);

    rewind(fp);
    fread(*ptr, size, 1, fp);
    fclose(fp);

    return size;
}
