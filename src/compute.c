#include "compute.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "array_io.h"

#include "SETTINGS.h"

int** data; //main data array
long exp_count; //exposure count
pthread_mutex_t data_m; //mutex for both data and exp_count

bool stop; //true if the rendering should be stopped
pthread_mutex_t stop_m; //mutext for stop

int main(int argc, char* argv[])
{   
    //allocate main data array
    data = malloc(sizeof(double*) * WIDTH_PX);
    for (int x = 0; x < WIDTH_PX; x++) {
        data[x] = malloc(sizeof(double) * HEIGHT_PX);
    }
    
    //initialize data values
    FILE* f = fopen(PDATA_LOC, "r");
    if (f == NULL) {
        //initialize to zero
        for (int x = 0; x < WIDTH_PX; x++) {
            for (int y = 0; y < HEIGHT_PX; y++) {
                data[x][y] = 0.0;
            }
        }
    } else {
        //initialize to contents of file
        read_array((void**)data, WIDTH_PX, HEIGHT_PX, sizeof(int), PDATA_LOC); //TODO change this function to take a file pointer
    }
    
    //calculate exp_count
    exp_count = 0;
    for (int x = 0; x < WIDTH_PX; x++) {
	for (int y = 0; y < HEIGHT_PX; y++) {
	    exp_count += data[x][y];
	}
    }

    //set up non-user globals
    pthread_mutex_init(&data_m, NULL);
    stop = false;
    pthread_mutex_init(&stop_m, NULL);
    
    //start engine threads
    pthread_t working_thread[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&working_thread[i], NULL, engine, NULL);
        printf("Thread %d started.\n", i);
    }
    
    //cmd loop
    bool done = false;
    do {
    
        //get the user input
        char* input = malloc(sizeof(char) * 80);
        fgets(input, 80, stdin);
        printf("%s", input);
        
        //make decision based on user input
        if (strcmp(input, "exit\n") == 0) {
            done = true;
        } else if (strcmp(input, "stat\n") == 0) {
            pthread_mutex_lock(&data_m);
            printf("%ld\n", exp_count);
            pthread_mutex_unlock(&data_m);
        }
        
    } while (!done);
    
    //send indication that threads should stop
    pthread_mutex_lock(&stop_m);
    stop = true;
    pthread_mutex_unlock(&stop_m);
            
    //wait for threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(working_thread[i], NULL);
    }
    
    //save data to file
    printf("Saving data to file...\n");
    write_array((void**)data, WIDTH_PX, HEIGHT_PX, sizeof(int), PDATA_LOC);
    printf("done.\n");
    
    return EXIT_SUCCESS;
}


void* engine(void* p)
{
    bool exit = false;
    while (!exit) {
    
        //get a random point that meets the correct set criteria
        double rnd_x;
        double rnd_y;
        double MIN = -2.0;
        double MAX = 2.0;
        bool invalid = true;
        while (invalid) {
            rnd_x = (rand() * (MIN - MAX)) / RAND_MAX + MAX;
            rnd_y = (rand() * (MIN - MAX)) / RAND_MAX + MAX;
            invalid = IN_SET == (*FORMULA)(rnd_x, rnd_y, false);
        }
        
        //plot the point
        (*FORMULA)(rnd_x, rnd_y, true);
    
        //check if an exit should occur
        pthread_mutex_lock(&stop_m);
        exit = stop;
        pthread_mutex_unlock(&stop_m);
    }
    
    return NULL;
}

bool is_pow2_pc(double x_0, double y_0, bool plot)
{
    double x = 0.0;
    double y = 0.0;
    int current_iter = 0;
    
    double xx = x * x;
    double yy = y * y;
    
    do {
        current_iter++;
        
        //calculate next value
        double new_x = xx - yy + x_0;
        y = 2 * x * y + y_0;
        x = new_x;
        
        //place exposure
        if (plot && x >= MIN_X && x < MAX_X && y >= MIN_Y && y < MAX_Y) {
            expose(x, y);
        }
        
        //calculate squares of re and img parts of new value
        xx = x * x;
        yy = y * y;
        
    } while (current_iter <= MAX_ITER && xx + yy <= BAILOUT);
    
    return xx + yy <= BAILOUT;
}

bool is_pow4_pc(double x_0, double y_0, bool plot)
{
    //(a + bi)^3 = (aaa - abb) + (aab - bbb + 2aab + 2abb)i

    double x = 0.0;
    double y = 0.0;
    int current_iter = 0;
    
    double xx = x * x;
    double yy = y * y;
    
    do {
        current_iter++;
        
        //calculate next value
        double new_x = xx*xx - 6*xx*yy + yy*yy + x_0;
        y = 4*xx*x*y - 4*x*yy*y + y_0;
        x = new_x;
        
        //place exposure
        if (plot && x >= MIN_X && x < MAX_X && y >= MIN_Y && y < MAX_Y) {
            expose(x, y);
        }
        
        //calculate squares of re and img parts of new value
        xx = x * x;
        yy = y * y;
        
    } while (current_iter <= MAX_ITER && xx + yy <= BAILOUT);
    
    return xx + yy <= BAILOUT;
}

void expose(double x, double y)
{
    //work out which part of the array should be incremented
    int x_index = (double)WIDTH_PX / (MAX_X - MIN_X) * (x - MIN_X);
    int y_index = -(double)HEIGHT_PX / (MAX_Y - MIN_Y) * (y + MIN_Y);
    
    //TODO debugging, remove for final
    if (x_index < 0 || x_index > WIDTH_PX || y_index < 0 || y_index > HEIGHT_PX) {
        printf("ERROR: in expose, array out of bound - (%f,%f)=>(%d,%d)\n", x, y, x_index, y_index);
    }
    
    //expose the array and exp_count
    pthread_mutex_lock(&data_m);
    exp_count++;
    data[x_index][y_index]++;
    pthread_mutex_unlock(&data_m);
    return;
}

