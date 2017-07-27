#include "bmp_writer.h"

#include <stdio.h>

int write_bmp(uint32_t** data, int width, int height, char* fname)
{
    int bytes_at_eor; //number of bytes to append to end of row (each row must be a multiple of 4 bytes)
    int bytes_of_data; //the number of bytes in the file containing the bitmapped data.
    int bytes_in_head; //the number of bytes that are in the header of the bitmap file.
    
    bytes_at_eor = (4 - ((3 * width) % 4)) % 4;
    bytes_of_data = (3 * width + bytes_at_eor) * height;
    bytes_in_head = 0x36;
    
    //open output file
    FILE* f = fopen(fname, "w");
    
    //write header to file
    uint32_t d;
    
    //write magic number
    d = 0x4D42;
    fwrite(&d, 2, 1, f);
    
    //write filesize
    d = bytes_of_data + bytes_in_head;
    fwrite(&d, 4, 1, f);
    
    //app specific (0x00000000)
    d = 0x00;
    fwrite(&d, 4, 1, f);
    
    //pixel data offset
    d = 0x36;
    fwrite(&d, 4, 1, f);
    
    //number of header bytes from this point onwards
    d = 0x28;
    fwrite(&d, 4, 1, f);
    
    //width and height
    fwrite(&width, 4, 1, f);
    fwrite(&height, 4, 1, f);
    
    //number of color planes
    d = 0x01;
    fwrite(&d, 2, 1, f);
    
    //bits per pixel
    d = 24;
    fwrite(&d, 2, 1, f);
    
    //compression type (none)
    d = 0x00;
    fwrite(&d, 4, 1, f);
    
    //amount of bitmapped data
    d = bytes_of_data;
    fwrite(&d, 4, 1, f);
    
    //resolution
    d = 2835;
    fwrite(&d, 4, 1, f); //horizontal
    fwrite(&d, 4, 1, f); //vertical
    
    //colors in pallet (not using pallet)
    d = 0;
    fwrite(&d, 4, 1, f);
    
    //colors that are important (all are important)
    fwrite(&d, 4, 1, f);
    
    //write the bitmapped data
    for (int h = height - 1; h >= 0; h--) {
        for (int w = 0; w < width; w++) {    
            d = data[w][h];
            fwrite(&d, 1, 3, f);
        }
        d = 0x00;
        fwrite(&d, 1, bytes_at_eor, f);
    }
    
    
    //close the file
    fclose(f);
    return 0;
}

