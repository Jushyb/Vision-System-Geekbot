#ifndef UTILS_H
#define UTILS_H


using namespace std;

void vectorPrint(float* u , int rows); 

void vectorScale(float* u, int rows, float alpha , float* v); 

// class changes V
void matrixMult(float* a, int rows_a, int cols_a, float* b, int rows_b, int cols_b, float* c);


#endif

