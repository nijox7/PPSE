#include <stdio.h>
#include <stdlib.h>
#include "gaussian.h"
#include "math.h"
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){
//   1/ (sqrt(2*M_PI) * sigma) * exp (-
// }

// int prob_gauss(float x, float y, float sigma){
//   1/ (sqrt(2*M_PI) * sigma) * exp(-())
// }

// int Gaussian(float x, float sigma){
//   srand(time(NULL));

//   const int n = 50;
//   float sum = 0;

//   for (int i = 0; i < n; i++){
//     float n = rand() % sigma*sigma; // generate random number between 0 and sigma^2.
//     float prob =  prob_gauss(x, x+n, sigma);
//     sum += sigma * prob;
//   }

//   return sum/n;
// }

float normal_law(){
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
  return sqrt(-2.0 * logf(u[0])) * cosf(2*M_PI*u[1]);
  // return x * sqrt((-2.0*logf(u))/u); // Z0
}


int main(){
  srand(time(NULL));
  FILE* f = fopen("gaussian.csv", "w+");
  if (!f) {
    printf("WTFF");
    exit(1);
  }

  fprintf(f, "[X;Y]\n");

  for (int i = 0; i < 100; i++){
    fprintf(f, "%d;%f\n", i, normal_law());
  }

  fclose(f);
  return 0;
}