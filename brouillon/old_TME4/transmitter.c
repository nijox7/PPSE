#include "transmitter.h"
#include <time.h>

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