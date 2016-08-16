#!/usr/bin/env python3

import sys

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
        while True:
            line = sys.stdin.readline().strip()
            if line == 'ENDCHAR':
                break

            hex_data = int(line, 16)
            print(format(hex_data, '016b').translate({
                ord('0'): '..', ord('1'): '##'
            }))
