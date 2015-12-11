#include <stdio.h>
#include <stdint.h>
#include <string.h>

void *
get_bitmap_font(void *buf, unsigned char *bytes[], size_t offset, size_t size)
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
    unsigned char *str = "#!\xc4\xfe\xbe\xb2\xd6\xc2\xd4\xb6";
    size_t rows = 16, cols = 16;
    size_t offset_size = rows * cols / 8;
    uint8_t font_data[offset_size];

    for (unsigned char *beg = str, *end = str + strlen(str); beg != end;) {
        get_bitmap_font(font_data, &beg, 0, offset_size);
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
