#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <arm_neon.h>
#include <time.h>

void print_int8(int8_t bin){
  int bit;
  int bits[8];

  // converts in table
  for (int i = 0; i < 8; i++){
    bit = 0b1 & bin;
    if (bit)  bits[i] = 1;
    else      bits[i] = 0;
    bin = bin >> 1;
  }

  // print table
  printf("0b");
  for (int i=7; i>=0; i--) {printf("%d", bits[i]);}
  printf("\n");
}

void old_quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
  /*
    f : size of floating part (0 to 8)
    s : total size used in L8 (1 to 8)
    s >= f !!
  */
  if (s < f) {
    printf("Error in quantizer_transform, s = %d > f = %d!!\n", s, f);
    exit(1);
  }

  int temp = 0;
  float L;
  int8_t L8;

  for (int i = 0; i < N; i++){
    L = L_N[i];
    L8 = 0b00000000; // initialize L8

    float intPart;
    float decPart = modff(L, &intPart);
    printf("intPart = %f, decPart = %f\n", intPart, decPart);
    if (intPart < 0){
      intPart = fabs(intPart);
      decPart = fabs(decPart);
    }
    printf("ABSOLUTE VALUE intPart = %f, decPart = %f\n", intPart, decPart);

    printf("\nDec part\n");
    // writing dec part
    for (int j = 0; j < f; j++){
      float power = pow(2, -(j+1)); // power of 2 from -1 to -f
      printf("decPart = %f, power = %f\n", decPart, power);
      print_int8(L8);
      printf("indice de décalage : f - (j+1) = %d\n", f - (j+1));
      if (decPart >= power) {
        decPart -= power;
        L8 = L8 | (0b1 << (f - (j+1)));
      }
    }

    printf("\nInt part\n");
    // writing int part
    for (int j = s-f-1; j >= 0; j--){
      float power = pow(2, j);
      printf("intPart = %f, power = %f\n", intPart, power);
      print_int8(L8);
        printf("indice de décalage : (f + j) = %d\n", f + j);
      if (intPart >= power){
        intPart -= power;
        L8 = L8 | (0b1 << (f + j));
      }
    }

    L8_N[i] = L8;
  }
}

void quantizer_transform8(const float *L_N, int8_t *L8_N, size_t N, size_t s, size_t f){
  /*
    f : size of floating part (0 to 8)
    s : total size used in L8 (1 to 8)
    s >= f !!
  */
  float intPart;

  if (s < f) {
    printf("Error in quantizer_transform, s = %d > f = %d!!\n", s, f);
    exit(1);
  }

  float rescale = 1.5;
  for (int i = 0; i < N; i++){
    printf("L8_N[%d] = %f\n", i, L_N[i]);
    L8_N[i] = (int) fminf(fmaxf((pow(2, f) * L_N[i]) / rescale, -pow(2, s-1)), pow(2, s-1) - 1);
    printf("L_N[%d] = %f\n", i, ((float) L8_N[i]) * rescale);
  }
}

// hard decoder8: first hard decides each LLR and then makes a majority vote
void codec_repetition_hard_decode8(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps){
  int8_t temp_K[K]; // temp array to save the vote results

  // initialize votes to 0
  for (int k = 0; k < K; k++){ temp_K[k] = 0; }

  // count each vote
  for (int j = 0; j < K; j++){
    for (int i = 0; i < n_reps; i++){
      if (L8_N[i*K + j] >= 0.0) temp_K[j]++;
      else                     temp_K[j]--;
    }
  }

  // decides by majority
  for (int i = 0; i < K; i++){
    if (temp_K[i] > 0) V_K[i] = 0;
    else               V_K[i] = 1;
  }
}

uint8x16_t vcltzq_s8(int8x16_t a){
  return vceqq_s8(vandq_s8(a, vdupq_n_s8(-128)), vdupq_n_s8(-128));
}

// hard decoder: first hard decides each LLR and then makes a majority vote
void codec_repetition_hard_decode8_neon(const int8_t *L8_N, uint8_t *V_K, size_t K, size_t n_reps){
  int8_t count_K[K]; // counter array to save the vote results
  int k;

  // initialize votes to 0
  for (k = 0; k + 16 <= K; k += 16){
    int8x16_t lane = vdupq_n_s8(0); // lane with all zeros
    vst1q_s8(count_K + k, lane);    // initialize count_K to 0
  }
  for (; k < K; k++){ count_K[k] = 0;} // initialization remaining without SIMD

  // count each vote
  for (int i = 0; i < n_reps; i++){
    for (k = 0; k + 16 <= K && (i*K + k + 16 <= (i+1)*K); k += 16){ 
      int8x16_t lane1, lane2, lane_minus_one;
      // if >  0 count - 1
      // if <= 0 count + 1
      lane1 = vld1q_s8(L8_N + i*K + k);             // lane1 <= L8
      lane1 = (int8x16_t) vcltzq_s8(lane1);         // contains 0 or 1
      lane_minus_one = vdupq_n_s8(-1);              // initialize every elements to -1
      lane2 = vaddq_s8(lane1, lane_minus_one);      // contains 0 or -1
      lane1 = vaddq_s8(lane1, lane2);               // contains 1 or -1, res
      lane2 = vld1q_s8(count_K + k);                // lane2 <= res
      vst1q_s8(count_K + k, vaddq_s8(lane1, lane2)); // count <= res + count
    }
    for (; k < K; k++){ // remainers without SIMD
      if (L8_N[i*K + k] >= 0.0) count_K[k]++;
      else                      count_K[k]--;
    }
  }

  // decides by majority
  for (k = 0; k + 16 <= K; k += 16){ // with SIMD
    vst1q_s8(V_K + k, (int8x16_t) vcltzq_s8(vld1q_s8(count_K + K)));
  }
  for (; k < K; k++){ // remainers without SIMD
    int8_t res = count_K[k] > 0 ? 0 : 1;
    V_K[k] = res;
  }
}

void main_quantizer(){
  float L_N = 79;
  // const float L_N = 0.9375;
  int8_t L8_N;
  size_t N = 1;
  size_t s = 8;
  size_t f = 1;
  quantizer_transform8(&L_N, &L8_N, N, s, f);
  printf("\nFinal number:\n");
  print_int8(L8_N);
}

void main_neon(){
  srand(time(NULL));

  const int K = 64;
  const int nreps = 4;
  const int N = nreps * K;

  uint8_t V_K[K];
  uint8_t V_K_neon[K];
  uint8_t L8_N[N];
  for (int i = 0; i < N; i++){
    L8_N[i] = rand() % 100;
    L8_N[i] -= 50;
    // printf("L8_N[%d] = %d\n", i, L8_N[i]);
  }

  codec_repetition_hard_decode8(L8_N, V_K, K, nreps);
  codec_repetition_hard_decode8_neon(L8_N, V_K_neon, K, nreps);

  for (int i = 0; i < K; i++){
    printf("i = %d, V_K = %d, V_K_neon = %d\n", i, V_K[i], V_K_neon[i]);
  }
  // for (int i = 0; i < K; i++){
  //   printf("V_K[%d] = %d\n", i, V_K[i]);
  // }

  // int8x16_t lane;
  // lane = vdupq_n_s8(1);
  // vcltzq_s8(lane);
}

int main(){
  // main_quantizer();
  main_neon();
  return 0;
}