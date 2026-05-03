#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"
#include "math.h"
#include <time.h>

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

void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
  /*
    f : size of floating part (0 to 8)
    s : total size used in L8 (1 to 8)
    s >= f !!
  */
  if (s < f) {
    printf("Error in quantizer_transform, s>f! s=%d, f=%d\n", s, f);
    exit(1);
  }

  int temp = 0;
  float L;
  int8_t L;
  for (int i = 0; i < N; i++){
    L = L_N[i];
    L8 = L8_N[i];
    L8 = 0b00000000; // initialize L8

    // copy float part from L to L8
    for (int ff = f - 1 ; ff > 0; ff--){
      L8 |= ((L >> ff) & 0b1) << ff;
    }

    // convert integer part
    temp += pow(2, )
  }
}