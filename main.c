#include <stdio.h>
#include <stdint.h>
#include <string.h>

void *
get_bitmap_font(void *buf, const unsigned char *bytes, size_t offset)
{
    size_t offset_size = 16 * 16 / 8;

    // 先求区位码
    size_t code_point =
        (bytes[0] - 0xA1)    // 区码
        * 94                 // 每区的字符数
        + (bytes[1] - 0xA1)  // 位码
    ;

    // 然后再计算其在区位码二维表中的位置，进而得出此字符在文件中的偏移
    if (bytes[0] >= 0xA1) {
        offset += code_point * offset_size;
    } else {
        offset += (bytes[0] + 156 - 1) * offset_size;
    }

    FILE *fp = fopen("res/font/HZK16", "rb");
    fseek(fp, offset, SEEK_SET);
    fread(buf, 1, offset_size, fp);
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

    uint16_t font_data[16];
    for (size_t i = 0; i < len; i += 2) {
        get_bitmap_font(font_data, str + i, 0);
        putchar('\n');
        for (int row = 0; row < 16; row++) {
            for (int col = 7; col >= 0; col--) {
                fputs(((font_data[row] >> col) & 1)? "##": "  ", stdout);
            }
            for (int col = 15; col >= 8; col--) {
                fputs(((font_data[row] >> col) & 1)? "@@": "  ", stdout);
            }
            putchar('\n');
        }
    }
    return 0;
}
