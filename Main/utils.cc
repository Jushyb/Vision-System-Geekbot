#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

void vectorPrint(float* u , int rows)
{
  for(int i=0;i<rows;i++)
  {
    printf( "%7.1f \n",u[i]);
  }
  printf("\n");

}

void vectorScale(float* u, int rows, float alpha , float* v)
{
  for(int i=0; i<rows; i++)
  {
    v[i]=alpha*u[i];
  }
}

// added in lecture V
void matrixMult(float*a, int rows_a, int cols_a, float*b, int rows_b, int cols_b, float*c)
{
  int index = 0;
  int rows = rows_a;
  int cols = cols_b;

  if(cols_a == rows_b)
  {
    for(int row = 0; row < rows; row++)
    {
      for(int col = 0; col < cols_b; col++)
      {
        index = row*cols + col;
        c[index] = 0.0;
        for(int k = 0; k < cols_a; k++)
        {
          c[index]=c[index]+a[row*cols_a + k] *b[col+k*cols_b];
        }
      }
    }
  }
  else
  {
    printf("Can't multiply. Dimensions are incorrect\n");
    exit(0);
  }

}

// added for hw1

//---------------------------------------------------------------
void vectorDotProduct(float* a, float* b, int size, float& sum)
{
  sum = 0.0;
  for(int i = 0; i < size; i++)
  {
    sum += a[i] * b[i];
  }
}
//---------------------------------------------------------------

void vectorSubtract(float* a, int rows, float* b, float* result)
{
  for (int i = 0; i < rows; i++)
  {
    result[i] = a[i] - b[i];
  }
}
//---------------------------------------------------------------
void vectorNorm(float*a, int rows, float& norm)
{
 float sum = 0.0;
 for(int i = 0; i < rows; i++)
 {
  sum+= a[i] * a[i];
 } 
 norm = sqrt(sum);
}
//---------------------------------------------------------------