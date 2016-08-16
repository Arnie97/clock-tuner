#!/usr/bin/env python3

import sys


def msb(n):
    'Return position of the most significant bit in an integer.'
    i = 0
    while n:
        i += 1
        n >>= 1
    return i


for line in sys.stdin:
    group = line.split()

    if group[0] == 'FONT':
        print(' '.join(group[1:]))
    elif group[0] == 'STARTFONT':
        count = ascii_count = gb_count = 0
        storage = {}
    elif group[0] == 'ENDFONT':
        ascii_total, gb_total = 95, (6763 + 682)
        sys.stderr.write(
            '%d utilized glyphs amongst %d parsed ones.\n'
            ' ASCII coverage: %.2f%% (%d hits, %d misses)\n'
            'GB2312 coverage: %.2f%% (%d hits, %d misses)\n' % (
                ascii_count + gb_count, count,
                ascii_count / ascii_total * 100,
                ascii_count, ascii_total - ascii_count,
                gb_count / gb_total * 100,
                gb_count, gb_total - gb_count
            )
        )
        break
    elif group[0] == 'SIZE':
        pixel_size = int(group[1])
        hex_data_size = int((pixel_size + 7) / 8) * 8
    elif group[0] == 'STARTCHAR':
        glyph_name = group[1]
        count += 1

    elif group[0] == 'ENCODING':
        code_point = int(group[1])
        try:
            euc_cn = chr(code_point).encode('GB2312')
        except UnicodeEncodeError:
            while line != 'ENDCHAR':
                line = sys.stdin.readline().strip()
            continue
        except ValueError as e:
            print('Unknown glyph:', glyph_name)
            continue

        if code_point <= 0x7F:
            ascii_count += 1
            key = code_point
            charset = 'ASCII: %#X' % code_point
        else:
            gb_count += 1
            key = int.from_bytes(euc_cn, 'big')
            charset = 'EUC-CN: %#X  GB2312: %s' % (
                key,
                ''.join(format(byte - 0xA0, '02d') for byte in euc_cn)
            )
        print('Unicode: %#X  %s' % (code_point, charset))

    elif group[0] == 'BBX':
        width, height, x0, y0 = map(int, group[1:5])
        print(width, height, x0, y0)

    elif group[0] == 'BITMAP':
        buffer, offset = [], 0
        while True:
            line = sys.stdin.readline().strip()
            if line != 'ENDCHAR':
                hex_data = int(line, 16)
                buffer.append(hex_data)
                offset = max(offset, msb(hex_data))

            else:
                glyph = []
                offset -= width
                for hex_data in buffer:
                    if offset > 0:
                        hex_data >>= offset
                    else:
                        hex_data <<= -offset
                    print(
                        format(hex_data, '0%db' % width).
                        translate({ord('0'): '..', ord('1'): '##'})
                    )

                storage[key] = buffer
                break


with open('HZK%d' % pixel_size, 'wb') as f:
    for i in range(0xA1, 0xA1 + 87):
        for j in range(0xA1, 0xA1 + 94):
            # Fill missing glyphs with zero
            hex_data = storage.get((i << 8) | j, [0] * 8)

            # Add padding for items with spaces
            while len(hex_data) < 8:
                hex_data.insert(0, 0)
            f.write(bytes(hex_data))
