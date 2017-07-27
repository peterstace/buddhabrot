#ifndef BMP_WRITER_H
#define BMP_WRITER_H

#include <stdint.h>

/**
 * Writes a bitmap to file. The bitmap will have 24 bits per pixel.
 * @param data 2D array of values to write. The 2 MSBs are ignored: 0x??RRGGBB
 * @param width in pixels.
 * @param height in pixels.
 * @param fname the filename to write to.
 * @return 0 if there are no problems, 1 otherwise.
 */
int write_bmp(uint32_t** data, int width, int height, char* fname);

#endif

