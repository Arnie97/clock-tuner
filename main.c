#include <stdio.h>
#include <stdint.h>
#include <string.h>

void *
get_bitmap_font(void *buf, const unsigned char *bytes, size_t offset, size_t size)
{
    // 先求区位码
    size_t code_point =
        (bytes[0] - 0xA1)    // 区码
        * 94                 // 每区的字符数
        + (bytes[1] - 0xA1)  // 位码
    ;

    // 然后再计算其在区位码二维表中的位置，进而得出此字符在文件中的偏移
    if (bytes[0] >= 0xA1) {
        offset += code_point * size;
    } else {
        offset += (bytes[0] + 156 - 1) * size;
    }

    FILE *fp = fopen("res/font/HZK16", "rb");
    fseek(fp, offset, SEEK_SET);
    fread(buf, 1, size, fp);
    fclose(fp);
    fp = NULL;
    return buf;
}

int
main(int argc, const char *argv[])
{
    unsigned char *str = "\xc4\xfe\xbe\xb2\xd6\xc2\xd4\xb6";
    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
        printf("0x%x ", str[i]);
    }
    putchar('\n');

    size_t rows = 16, cols = 16;
    size_t offset_size = rows * cols / 8;
    uint8_t font_data[offset_size];

    for (size_t i = 0; i < len; i += 2) {
        get_bitmap_font(font_data, str + i, 0, offset_size);
        putchar('\n');

        uint8_t *ptr = font_data, pos = 7;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                fputs(((*ptr >> pos) & 1)? "@@": "  ", stdout);
                if (pos) {
                    pos--;
                } else {
                    pos = 7;
                    ptr++;
                }
            }
            putchar('\n');
        }
    }
    return 0;
}
