#include <stdio.h>
#define N 10

int main(){
  double res1, res2;
  double pow2[N], pow3[N];
  double sum_pow2=0.0, sum_pow3=0.0;
  double power2 = 2.13;
  double power3 = 3.21;

  for (int i=0; i<=N; i++){
    if (i==0){
      pow2[i] = 1.0;
      pow3[i] = 1.0;
    }
    else{
      pow2[i] = power2*pow2[i-1];
      pow3[i] = power3*pow3[i-1];
    }
    sum_pow2 += pow2[i];
    sum_pow3 += pow3[i];
  }

  for (int i=0; i<=N; i++){
    printf("2^%d = %f\n", i, pow2[i]);
    printf("3^%d = %f\n", i, pow3[i]);
  }
  printf("Sum of 2^i from 0 to %d = %f\n", N, sum_pow2);
  printf("Sum of 3^i from 0 to %d = %f\n", N, sum_pow3);

  printf("\nCalculating the result of the 3's power sum multiplied by the 2's power sum\n");

  // First calculation method
  res1 = sum_pow2*sum_pow3;
  printf("First calculation method: %f\n", res1);

  // Second calculation method
  res2 = 0;
  for (int i = 0; i <= N; i++){
    for (int j = 0; j <= N; j++){
      res2 += pow2[i]*pow3[j];
    }
  }
  printf("Second calculation method: %f\n", res2);

  return 0;
}