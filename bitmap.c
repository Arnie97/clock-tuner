#include <time.h>
#include "util.h"


uint8_t *
generate_random_image(uint32_t width, uint32_t height)
{
    const uint16_t
    bpp           = 1;

    const uint32_t
    bytes_per_row = (width * bpp + 31) / 32 * 4,
    data_size     = bytes_per_row * height;

    uint8_t *buf = alloc(data_size);

    /* generate grayscale of each pixel and save it to buf array */
    srand(time(0));
    for (uint32_t x = 0; x < bytes_per_row; x++) {
        for (uint32_t y = 0; y < height; y++) {
            uint32_t offset = y * bytes_per_row + x;
            for (int bit = 0; bit < 8; bit++) {
                if (rand() >= RAND_MAX / 2) {
                    buf[offset] |= (1 << bit);
                }
            }
        }
    }
    return buf;
}


int
write_1_bit_bmp(const char *file, uint32_t width, uint32_t height, uint8_t *data)
{
    const uint16_t
    bpp               = 1,  // bit of colors per pixel; 1 for monochrome
    planes            = 1;  // the number of color planes; must be 1

    const uint32_t
    file_header_size  = 14,
    info_header_size  = 40,
    palette_size      = (1 << bpp) * 4,
                            // 1 bit per pixel,
                            // 2 = 2 ** 1 colors in the palette,
                            // 4 channels per color,
                            // 8 = 2 * 4 bytes for the palette
    offset_bits       = file_header_size + info_header_size + palette_size,
    bytes_per_row     = (width * bpp + 31) / 32 * 4,
    data_size         = bytes_per_row * height,
    file_size         = offset_bits + data_size,

    compression       = 0,  // compression disabled
    image_size        = 0,  // set to 0 when no compression is used
    colors_used       = 2,  // number of colors used by the bitmap
    colors_important  = 0,  // number of colors that are important
    x_pels_per_meter  = 0,  // number of pixels per meter in x axis
    y_pels_per_meter  = 0;  // number of pixels per meter in y axis

#if _DEBUG
    printf("scale %d x %d x %d\n", width, height, bpp);
    printf("bytes_per_row = %d\n", bytes_per_row);
    printf("offset_bits   = %d\n", offset_bits);
    printf("data_size     = %d\n", data_size);
    printf("file_size     = %d\n", file_size);
#endif

    FILE *fp = open(file, "wb");
    #define FWRITE(x) fwrite(&(x), 1, sizeof(x), fp)

    /* bmp file header */
    fwrite("BM",   1, 2, fp);  // magic number
    FWRITE(file_size);
    fwrite("\0\0", 2, 2, fp);  // reserved numbers; must be zeros
    FWRITE(offset_bits);

    /* bitmap info header */
    FWRITE(info_header_size);

    FWRITE(width);
    FWRITE(height);
    FWRITE(planes);
    FWRITE(bpp);

    FWRITE(compression);
    FWRITE(image_size);
    FWRITE(x_pels_per_meter);
    FWRITE(y_pels_per_meter);

    FWRITE(colors_used);
    FWRITE(colors_important);

    /* palette with 2 colors in RGBA */
    fwrite("\xff\xff\xff\xff", 1, 4, fp);  // color 0: white
    fwrite("\x00\x00\x00\xff", 1, 4, fp);  // color 1: black

    /* image pixel data */
    fwrite(data, 1, data_size, fp);

    fclose(fp);
    return 0;
}


#if _DEBUG
int
main(int argc, char *argv[])
{
    const uint32_t
    width         = 131,
    height        = 64;

    const unsigned char
    output_file[] = "__ADK__.BMP";

    puts("Generating a random monochrome bitmap file...");
    uint8_t *bytes = generate_random_image(width, height);
    int ret = write_1_bit_bmp(output_file, width, height, bytes);
    printf("Written to file %s.\n", output_file);

    free(bytes);
    return ret;
}
#endif
