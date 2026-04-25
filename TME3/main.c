#include <stdio.h>
#include "transmitter.h"
#include "receiver.h"
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

int main(int argc, char** argv){
  char opt;
  float m = 0;
  float M = 12;
  float s = 1;
  uint e = 100;
  uint K = 8;
  uint N = 32;
  char D[15] = "rep-hard";

  while((opt = getopt(argc, argv, "m:M:s:e:K:N:D:")) != -1){
    switch(opt){
      case 'm':
        m = atof(optarg);
        break;
      case 'M':
        M = atof(optarg);
        break;
      case 's':
        s = atof(optarg);
        break;
      case 'e':
        e = atoi(optarg);
        break;
      case 'K':
        K = atoi(optarg);
        break;
      case 'N':
        N = atoi(optarg);
        break;
      case 'D':
        strcpy(D, optarg);
        break;
    }
  }

  srand(time(NULL));
  for (float snr = m; snr < M; snr += s){
    uint64_t n_bit_errors = 0;
    uint64_t n_frame_errors = 0;
    uint64_t n_simu = 0;

    float snr_symb = snr + 10.0*log10f( ((float) K) / ((float) N) );
    float sigma = sqrt(1.0 / (2.0 * powf(10, snr_symb/10.0)));

    do {
      uint8_t U_K[K];
      uint8_t C_N[N];
      int32_t X_N[N];
      float Y_N[N];
      float L_N[N];
      uint8_t V_K[K];

      source_generate(U_K, K);
      codec_repetition_encode(U_K, C_N, K, N/K);
      modem_BPSK_modulate(C_N, X_N, N);

      channel_AWGN_add_noise(X_N, Y_N, N, sigma);
      modem_BPSK_demodulate(Y_N, L_N, N, sigma);
      codec_repetition_hard_decode(L_N, V_K, K, N/K);
      monitor_check_errors(U_K, V_K, K, &n_bit_errors, &n_frame_errors);

      n_simu++;
    } while(n_frame_errors < e);

    printf("n_bit_error / (n_simu * K) = %f\n", ((float) n_bit_errors) / (float) (n_simu * K));
    printf("n_frame_errors / n_simu = %f\n", ((float) n_frame_errors) / (float) n_simu);
    printf("\n");
  }

  return 0;
}