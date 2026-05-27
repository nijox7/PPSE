#include "receiver.h"
#include "math.h"
#include <time.h>
#include <stdio.h>
#include "math.h"
#include "gaussian.h"

// add white Gaussian noise
// void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){
//   srand(time(NULL));
//   float n;
//   for (int i = 0; i < N; i++){
//     n = (rand()%11 - 5) * sigma; // 0 to 10
//     printf("n=%f ", n);
//     Y_N[i] = X_N[i] + n; // add noise
//     printf("Y_N[i] = %f\n", Y_N[i]);
//   }
//   printf("\n");
// }

void channel_AWGN_add_noise(const int32_t *X_N, float *Y_N, size_t N, float sigma){
  float n;
  for (int i = 0; i < N; i++){
    n = normal_law();
    printf("n=%f ", n);
    Y_N[i] = X_N[i] + n; // add noise
    printf("Y_N[i] = %f\n", Y_N[i]);
  }
  printf("\n");
}

// demodulator, just copies Y_N in L_N for now
void modem_BPSK_demodulate(const float *Y_N, float *L_N, size_t N, float sigma){
  for(int i = 0; i < N; i++){
    L_N[i] = Y_N[i];
    printf("L_N[i] = %f\n", L_N[i]);
  }
  printf("\n");
}

// hard decoder: first hard decides each LLR and then makes a majority vote
void codec_repetition_hard_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps){
  int temp_K[K]; // temp array to save the vote results

  // initialize votes to 0
  for (int k = 0; k < K; k++){ temp_K[k] = 0; }

  // count each vote
  for (int j = 0; j < K; j++){
    for (int i = 0; i < n_reps; i++){
      if (L_N[i*n_reps + j] >= 0.0) temp_K[j]++;
      else                          temp_K[j]--;
    }
  }

  // decides by majority
  for (int i = 0; i < K; i++){
    if (temp_K[i] > 0) V_K[i] = 0;
    else               V_K[i] = 1;
    printf("V_K[i] = %d et tmpK[i] = %d\n", V_K[i], temp_K[i]);
  }
    printf("\n");
}

// soft decoder: computes the mean of each LLR to hard decide the bits
void codec_repetition_soft_decode(const float *L_N, uint8_t *V_K, size_t K, size_t n_reps){
  int temp_K[K]; // temp array to save the averages

  // initialize averages to 0
  for (int k = 0; k < K; k++){ temp_K[k] = 0; }

  // calculating the averages
  for (int j = 0; j < K; j++){
    for (int i = 0; i < n_reps; i++){
      temp_K[j] += L_N[i*n_reps + j];
    }
  }

  // makes the hard decision
  for (int i = 0; i < K; i++){
    if (temp_K[i] >= 0) V_K[i] = 0;
    else               V_K[i] = 1;
    printf("V_K[i] = %d\n", V_K[i]);
  }
  printf("\n");
}

// update `n_bit_errors` and `n_frame_errors` variables depending on `U_K` and `V_K`
void monitor_check_errors(const uint8_t *U_K, const uint8_t *V_K, size_t K, uint64_t *n_bit_errors, uint64_t *n_frame_errors){
  *n_bit_errors = 0;
  *n_frame_errors = 0;

  for (int i = 0; i < K; i++){
    if (U_K[i] != V_K[i]) (*n_bit_errors)++;
  }
  if (*n_bit_errors > 0) *n_frame_errors = 1;
  printf("Bit errors: %d\n", *n_bit_errors);
  printf("Frame errors: %d\n", *n_frame_errors);
}