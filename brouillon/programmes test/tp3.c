#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define SIZE 4
#define NREPS 2

// write into the buffer U_K
void source_generate(uint8_t *U_K, size_t K){
  srand(time(NULL));
  printf("SOURCE GENERATE\n");
    for (int i = 0; i < K; i++){
      U_K[i] = rand() % 2;
      printf("U_K[%d] = %u\n", i, U_K[i]);
    }
  printf("\n");
}

// write into the buffer U_K
void codec_repetition_encode(const uint8_t *U_K, uint8_t *C_N, size_t K, size_t n_reps){
  printf("CODEC\n");
  for (int i = 0; i < n_reps; i++){
    for (int j = 0; j < K; j++){
      C_N[i*K + j] = U_K[j];
      printf("C_N[%d] = %u\n", i, C_N[i*K + j]);
    }
  }
  printf("\n");
}

// read from C_N, write into X_N
void modem_BPSK_modulate(const uint8_t *C_N, int32_t *X_N, size_t N){
  
  printf("BPSK\n");
  for (int i = 0; i < N; i++){
    if (C_N[i] == 0)  X_N[i] = 1;  // 0 -> 1
    else              X_N[i] = -1; // 1 -> -1
    printf("X_N[%d] = %d\n", i, X_N[i]);
  }
  printf("\n");
}


/* TP2 */

#include <math.h>

double gaussian(float x, float sigma){
  for (int )
  1/ (sqrt(2*M_PI) * sigma) * exp (-
}

// add white Gaussian noise
void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){
  1/ (sqrt(2*M_PI) * sigma) * exp (-
}

// demodulator, just copies Y_N in L_N for now
void modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma);

// hard decoder: first hard decides each LLR and then makes a majority vote
void codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);

// soft decoder: computes the mean of each LLR to hard decide the bits
void codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps);

// update `n_bit_errors` and `n_frame_errors` variables depending on `U_K` and `V_K`
void monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors);

int main(){
  uint8_t *U_K = malloc(sizeof(uint8_t) * SIZE);
  uint8_t *C_N = malloc(sizeof(uint8_t) * SIZE*NREPS);
  uint32_t *X_N = malloc(sizeof(uint32_t) * SIZE*NREPS);

  source_generate(U_K, SIZE);
  codec_repetition_encode(U_K, C_N, SIZE, NREPS);
  modem_BPSK_modulate(C_N, X_N, SIZE*NREPS);
  
  free(U_K);
  free(C_N);
  free(X_N);
  return 0;
}