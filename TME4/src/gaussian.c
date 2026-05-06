#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"
#include "math.h"
#include <time.h>

#define RESCALE 1.5

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float normal_law(float sigma){
  /*
    Generating random number following normal's law
    ---
    Polar method, Box-Muller Transform
  */
  int n;
  float x, y;
  float u[2];

  for (int i = 0; i < 2; i++){
    do {
      // Generating uniform numbers
      n = rand()%2000; // integer between 0 and 1999
      x = ((float) n) / 1000.0; // float between 0 and 1,999
      n = rand()%2000; // integer between 0 and 999
      y = ((float) n) / 1000.0; // float between 0 and 1,999
    
      x += -1; // between -1,000 and 0,999
      y += -1; // between -1,000 and 0,999

      u[i] = (x*x) + (y*y);
    } while(u[i] >=1 || u[i] == 0);
  }
  return sigma * sqrt(-2.0 * logf(u[0])) * cosf(2*M_PI*u[1]);
}

void old_quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
  /*
    (!! Convert only positive numbers !!)
    f : size of floating part (0 to 8)
    s : total size used in L8 (1 to 8)
    s >= f !!

  */
  if (s < f) {
    printf("Error in quantizer_transform, s = %d > f = %d!!\n", s, f);
    exit(1);
  }

  int temp = 0;
  float L;
  int8_t L8;

  for (int i = 0; i < N; i++){
    L = L_N[i];
    L8 = 0b00000000; // initialize L8

    float intPart;
    float decPart = modff(L, &intPart);

    printf("\nDec part\n");
    // writing dec part
    for (int j = 0; j < f; j++){
      float power = pow(2, -(j+1)); // power of 2 from -1 to -f
      if (decPart >= power) {
        decPart -= power;
        L8 = L8 | (0b1 << (f - (j+1)));
      }
    }

    printf("\nInt part\n");
    // writing int part
    for (int j = s-f-1; j >= 0; j--){
      float power = pow(2, j);
      if (intPart >= power){
        intPart -= power;
        L8 = L8 | (0b1 << (f + j));
      }
    }

    L8_N[i] = L8;
  }
}

void print_int8(int8_t bin){
  /*
    Print 'bin' in format "0bxxxxxxxx"
  */
  int bit;
  int bits[8];

  // converts in table
  for (int i = 0; i < 8; i++){
    bit = 0b1 & bin;
    if (bit)  bits[i] = 1;
    else      bits[i] = 0;
    bin = bin >> 1;
  }

  // print table
  printf("0b");
  for (int i=7; i>=0; i--) {printf("%d", bits[i]);}
  printf("\n");
}

void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
  /*
    f : size of floating part (0 to 8)
    s : total size used in L8 (1 to 8)
    s >= f !!
  */
  float intPart;

  if (s < f) {
    printf("Error in quantizer_transform, s = %d > f = %d!!\n", s, f);
    exit(1);
  }

  for (int i = 0; i < N; i++){
    L8_N[i] = (int) fminf(fmaxf((pow(2, f) * L_N[i]) / RESCALE, -pow(2, s-1)), pow(2, s-1) - 1);
  }
}