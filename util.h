#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int write_1_bit_bmp(
    const char *file,
    uint32_t width, uint32_t height,
    uint8_t *data
);
extern void *alloc(size_t);
extern FILE *open(const char *, const char *);
extern size_t read_file(char **, const char *);

#endif
