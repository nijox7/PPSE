#include <stdio.h>
#include "transmitter.h"
#include "receiver.h"

#define SIZE 10
#define NREPS 2
#define SIGMA 0.25

int main(){
  uint8_t U_K[SIZE];
  uint8_t C_N[SIZE*NREPS];
  int32_t X_N[SIZE*NREPS];
  float Y_N[SIZE*NREPS];
  float L_N[SIZE*NREPS];
  uint8_t V_K[SIZE];
  uint64_t n_bit_errors;
  uint64_t n_frame_errors;

  source_generate(U_K, SIZE);
  codec_repetition_encode(U_K, C_N, SIZE, NREPS);
  modem_BPSK_modulate(C_N, X_N, SIZE*NREPS);

  channel_AWGN_add_noise(X_N, Y_N, SIZE*NREPS, SIGMA);
  modem_BPSK_demodulate(Y_N, L_N, SIZE*NREPS, SIGMA);
  codec_repetition_hard_decode(L_N, V_K, SIZE, NREPS);
  monitor_check_errors(U_K, V_K, SIZE, &n_bit_errors, &n_frame_errors);

  printf("Transmitter: ");
  for (int i = 0; i < SIZE; i++){
    printf("%d ", U_K[i]);
  }
  printf("\n");
  printf("Receiver:    ");
  for (int i = 0; i < SIZE; i++){
    printf("%d ", V_K[i]);
  }
  printf("\n");

  return 0;
}