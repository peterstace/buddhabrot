#include "render.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "array_io.h"
#include "bmp_writer.h"

#include "SETTINGS.h"

int main(int argc, char* argv[])
{
    //make sure file exists
    FILE* f = fopen(PDATA_LOC, "r");
    if (f == NULL) {
        printf("The file '%s' could not be opened, aborting.\n", PDATA_LOC);
        exit(1);
    }
    
    //allocate data
    int** data = NULL;
    data = malloc(sizeof(int*) * WIDTH_PX);
    for (int w = 0; w < WIDTH_PX; w++) {
        data[w] = malloc(sizeof(int) * HEIGHT_PX);
    }
    
    //get the data
    printf("Getting data...\n");
    int ret = read_array((void**)data, WIDTH_PX, HEIGHT_PX, sizeof(int), PDATA_LOC);
    switch (ret) {
        case 1: printf("I/O error while accessing '%s', aborting.\n", PDATA_LOC); exit(1);
        case 2: printf("Logic error inside file '%s', aborting.\n", PDATA_LOC); exit(1);
    }
    printf("done.\n");
    
    //get some statistics about the data
    int min = INT_MAX;
    int max = 0;
    for (int x = 0; x < WIDTH_PX; x++) {
        for (int y = 0; y < HEIGHT_PX; y++) {
            if (data[x][y] < min) {
                min = data[x][y];
            }
            if (data[x][y] > max) {
                max = data[x][y];
            }
        }
    }
    
    //transform the data
    for (int x = 0; x < WIDTH_PX; x++) {
        for (int y = 0; y < HEIGHT_PX; y++) {
            data[x][y] = (double)(data[x][y] - min) / (max - min) * 0xff;
            data[x][y] *= 0x010101; 
        }
    }
    
    //write out data to file
    write_bmp((uint32_t**)data, WIDTH_PX, HEIGHT_PX, OUTPUT_BMP);
    
    return EXIT_SUCCESS;
}

