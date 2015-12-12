#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void *
get_bitmap_font(void *buf, const unsigned char *bytes[], size_t offset, size_t size)
{
    if (0[*bytes] >= 0xA1) {
        size_t code_point =           // 先求区位码，然后再计算其在区位码二维表中的位置
            (0[*bytes] - 0xA1)        // 区码
            * 94                      // 每区的字符数
            + (1[*bytes] - 0xA1);     // 位码
        offset += code_point * size;  // 得出此字符在文件中的偏移
        *bytes += 2;                  // 全角字符
    } else {
        offset += (*bytes[0] + 156 - 1) * size;
        *bytes += 1;                  // 半角字符
    }
    FILE *fp = fopen("res/font/HZK12", "rb");
    fseek(fp, offset, SEEK_SET);
    fread(buf, 1, size, fp);
    fclose(fp);
    fp = NULL;
    return buf;
}

uint8_t *
bitmap_blit(uint32_t width, uint32_t height, const unsigned char *str)
{
    const size_t
    rows          = 12,
    cols          = 16,
    cols_         = 12,
    offset_size   = rows * cols / 8;

    const uint16_t
    bpp           = 1;

    const uint32_t
    bytes_per_row = (width * bpp + 31) / 32 * 4,
    data_size     = bytes_per_row * height;

    uint8_t *buf = malloc(data_size);
    if (!buf) {
        printf("Memory allocation FAILED!\n");
        return NULL;
    } else {
        memset(buf, 0, data_size);
    }

    uint8_t font_data[offset_size];
    size_t x = 0, y = height;
    for (const unsigned char *beg = str, *end = str + strlen(str); beg != end;) {
        get_bitmap_font(font_data, &beg, 0, offset_size);
        putchar('\n');
        uint8_t *ptr = font_data, pos = 7;
        for (size_t row = 0; row < rows; row++, y--) {
            for (size_t col = 0; col < cols; col++, x++) {
                printf("buf[%u,%u] = font[%u,%u] = %u\n", y, x, row, col, (*ptr >> pos) & 1);
                buf[y * bytes_per_row + (x >> 3)] |= ((*ptr >> pos) & 1) << (7 - x & 7);
                if (pos) {
                    pos--;
                } else {
                    pos = 7;
                    ptr++;
                }
            }
            x -= cols;
            putchar('\n');
        }
        if (x < width - cols_) {
            x += cols_;
            y += rows;
        } else {
            x = 0;
            y++;
        }
    }
    return buf;
}

int
main(int argc, const char *argv[])
{
    // parse_args();
    // read_file();

    const uint32_t
    width         = 131,
    height        = 64;

    const unsigned char
    text[]        = "#!\xc4\xfe\xbe\xb2\xd6\xc2\xd4\xb6",
    output_file[] = "__ADK__.BMP";

    uint8_t *buf = bitmap_blit(width, height, text);
    write_1_bit_bmp(output_file, width, height, buf);
    free(buf);

    return 0;
}
