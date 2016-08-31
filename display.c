/* Display manipulating module

Copyright (C) 2016 Arnie97

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <stdint.h>
#include <hpstdio.h>
#include <syscall.h>
#include "display.h"


void *
get_bitmap_font(const uint8_t **bytes)
{
    int page = 0[*bytes] - 0xA0;    // 区码
    int id   = 1[*bytes] - 0xA0;    // 位码
    if (page < 0) {                 // ISO/IEC 2022 G0区
        page = 3;                   // 用 GB 2312-1980 第3区中
        id = 0[*bytes] - ' ' - 1;   // 相应的全角字符代替半角字符
        *bytes += 1;                // 半角字符，指针移动一字节
    } else {                        // ISO/IEC 2022 GR区
        *bytes += 2;                // 全角字符，指针移动两字节
    }

    size_t offset = ((page - 1) * 94 + (id - 1)) * bytes_per_glyph;
    return (void *)offset;
}


const char *
bitmap_blit(const char *beg, const char *end)
{
    SysCall(ClearLcdEntry);
    int32_t x = left_margin, y = height - 1 - top_margin;
    while (beg != end) {
        uint8_t pos = 7, *ptr = get_bitmap_font((const uint8_t **)&beg);
        for (size_t row = 0; row < rows; row++, y--) {
            for (size_t col = 0; col < cols_storage; col++, x++) {
#ifdef _DEBUG
                printf("buf[%u,%u] = font[%u,%u] = %u\n", height - 1 - y, x, row, col, (*ptr >> pos) & 1);
#endif
                __display_buf[y * bytes_per_row + (x >> 3)] |= ((*ptr >> pos) & 1) << (~x & 7);
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
            break;
        }
    }
    return beg;
}
