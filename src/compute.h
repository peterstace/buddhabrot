#ifndef COMPUTE_H
#define COMPUTE_H

 #include <stdbool.h>

int main(int argc, char* argv[]);

void* engine(void* p);

void expose(double x, double y);

bool is_pow2_pc(double x_0, double y_0, bool plot);
bool is_pow4_pc(double x_0, double y_0, bool plot);

#endif

