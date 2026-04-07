#include "math.h"

// void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){
//   1/ (sqrt(2*M_PI) * sigma) * exp (-
// }

int prob_gauss(float x, float y, float sigma){
  1/ (sqrt(2*M_PI) * sigma) * exp(-())
}

int Gaussian(float x, float sigma){
  srand(time(NULL));

  const int n = 50;
  float sum = 0;

  for (int i = 0; i < n; i++){
    float n = rand() % sigma*sigma; // generate random number between 0 and sigma^2.
    float prob =  prob_gauss(x, x+n, sigma);
    sum += sigma * prob;
  }

  return sum/n;
}


int main(){

  return 0;
}