#include <string.h>
#include "util.h"

const char *file[] = {
    "res/font/HZK12",
    "gb2312.txt",
    "__ADK__.BMP"
};

static const size_t
rows          = 12,
cols_storage  = 16,
cols_real     = 13,

width         = 131,
height        = 64,
left_margin   = 1,
top_margin    = 0,
line_spacing  = 1,
bpp           = 1;


void *
get_bitmap_font(void *buf, const uint8_t **bytes, size_t offset, size_t size)
{
    size_t code_point;
    if (0[*bytes] >= 0xA1) {        // ISO/IEC 2022 GR区
        code_point =                // 计算字符在区位码二维表中的位置
            (0[*bytes] - 0xA1)      // 区码
            * 94                    // 每区的字符数
            + (1[*bytes] - 0xA1);   // 位码
        *bytes += 2;                // 全角字符，指针移动两字节
    } else {                        // ISO/IEC 2022 G0区
        code_point = 0[*bytes]      // 用相应的全角字符代替半角字符
            + (2 * 94 - ' ' - 1);   // GB 2312-1980，第3区
        *bytes += 1;                // 半角字符，指针移动一字节
    }
    offset += code_point * size;
    FILE *fp = open(file[0], "rb");
    fseek(fp, offset, SEEK_SET);
    fread(buf, 1, size, fp);
    fclose(fp);
    return buf;
}


unsigned
bitmap_blit(const char *beg, const char *end)
{
    const size_t
    offset_size = (rows * cols_storage + 7) / 8,
    bytes_per_row = (width * bpp + 31) / 32 * 4,
    data_size = bytes_per_row * height;

    unsigned page = 1;
    uint8_t *buf = alloc(data_size);
    initialize: memset(buf, 0, data_size);

    uint8_t font_data[offset_size];
    int32_t x = left_margin, y = height - 1 - top_margin;
    while (beg != end) {
        get_bitmap_font(font_data, (const uint8_t **)&beg, 0, offset_size);
        uint8_t *ptr = font_data, pos = 7;
        for (size_t row = 0; row < rows; row++, y--) {
            for (size_t col = 0; col < cols_storage; col++, x++) {
#ifdef _DEBUG
                printf("buf[%u,%u] = font[%u,%u] = %u\n", height - 1 - y, x, row, col, (*ptr >> pos) & 1);
#endif
                buf[y * bytes_per_row + (x >> 3)] |= ((*ptr >> pos) & 1) << (~x & 7);
                if (pos) {
                    pos--;
                } else {
                    pos = 7;
                    ptr++;
                }
            }
            x -= cols_storage;
        }
        if (x + cols_real <= width - cols_real) {  // next char
            x += cols_real;
            y += rows;
        } else if (y >= rows) {  // next line
            x = left_margin;
            y -= line_spacing;
        } else {  // next page
            write_1_bit_bmp(file[2], width, height, buf);
            printf("Page %u is full!\n", page++);
            puts("Press ENTER to proceed with the next page...");
            getchar();
            goto initialize;
        }
    }
    write_1_bit_bmp(file[2], width, height, buf);
    free(buf);
    printf("Well done. %u pages proceeded.\n", page);
    return page;
}


int
main(int argc, const char *argv[])
{
    for (int i = 1; i < argc; i++) {
        file[i - 1] = argv[i];
    }
    char *text;
    size_t len = read_file(&text, file[1]);
    bitmap_blit(text, text + len);
    free(text);
}
