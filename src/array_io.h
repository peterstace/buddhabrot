#ifndef ARRAY_IO_H
#define ARRAY_IO_H

#include <stdint.h>

/**
 * Write an array to a file.
 * @param data array to write to file.
 * @param w the width of the 2D array.
 * @param h the height of the 2D arary.
 * @param bpd bytes per datum.
 * @param fname the filename of the file to read from.
 * @return 1 if a problem occurs, 0 otherwise.
 */
int write_array(void** data, uint32_t w, uint32_t h, int bpd, char* fname);

/**
 * Read an array from file.
 * @param data a two dimensional array (should be preallocated). Destination of contents read from file.
 * @param w the width of the 2D array.
 * @param h the height of the 2D arary.
 * @param bpd bytes per datum.
 * @param fname the filename of the file to write to.
 * @return 1 if an I/O error occurs, 2 if a logical error occurs (such as widths or heights not matching), or 0
 * if everything goes okay.
 */
int read_array(void** data, uint32_t w, uint32_t h, int bpd, char* fname);

#endif

