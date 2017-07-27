#include "array_io.h"

#include "stdio.h"

int write_array(void** data, uint32_t w, uint32_t h, int bpd, char* fname)
{
    //open the file
    FILE* f = fopen(fname, "w");
    if (f == NULL) {
        return 1;
    }

    //write width and height
    fwrite(&w, 4, 1, f);
    fwrite(&h, 4, 1, f);
    
    //change data ptr type
    char** data_c = (char**)data;
    
    //write array data
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            fwrite((*(data_c + x)) + bpd * y, bpd, 1, f);
        }
    }
    
    fclose(f);
    return 0;
}

int read_array(void** data, uint32_t w, uint32_t h, int bpd, char* fname)
{
    //open the file
    FILE* f = fopen(fname, "r");
    if (f == NULL) {
        return 1;
    }
    
    //read in width and height to compare to those given to this function
    uint32_t read_w;
    uint32_t read_h;
    fread(&read_w, 4, 1, f);
    fread(&read_h, 4, 1, f);
    if (read_w != w || read_h != h) {
        return 2;
    }
    
    //change data ptr type
    char** data_c = (char**)data;
    
    //read data in
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            fread((*(data_c + x)) + bpd * y, bpd, 1, f);
        }
    }
    
    return 0;
}

