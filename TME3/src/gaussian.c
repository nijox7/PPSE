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