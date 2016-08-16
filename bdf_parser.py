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
        count = 0
    elif group[0] == 'ENDFONT':
        print('%d glyphs parsed' % count)
        break
    elif group[0] == 'SIZE':
        pixel_size = int(group[1])
        hex_data_size = int((pixel_size + 7) / 8) * 8
    elif group[0] == 'STARTCHAR':
        count += 1
    elif group[0] == 'ENCODING':
        encoding = int(group[1])
        print(hex(encoding))
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
                offset -= width
                print('\n'.join(
                    format(
                        hex_data >> offset
                        if offset > 0 else
                        hex_data << -offset,
                        '0%db' % width
                    ).translate({
                        ord('0'): '..', ord('1'): '##'
                    })
                    for hex_data in buffer
                ), '\n')
                break
