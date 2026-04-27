#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "transmitter.h"
#include "receiver.h"

int main(int argc, char** argv){
  char opt;
  float m = 0;
  float M = 12;
  float s = 1;
  uint e = 100;
  uint K = 8;
  uint N = 32;
  char D[15] = "rep-hard";
  FILE* filecsv = NULL;

  while((opt = getopt(argc, argv, "m:M:s:e:K:N:D:o:")) != -1){
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
      case 'o':
        filecsv = fopen(optarg, "w+");
        if (filecsv == NULL){
          printf("Erreur dans l'ouverture du fichier %s\n", optarg);
        }
        else{
          fprintf(filecsv, "Eb/N0,Es/N0,sigma,be,fe,fn,BER,FER,Time,  AverageTime\n");
        }
        break;
      default:;
    }
  }

  srand(time(NULL));
  for (float snr = m; snr < M; snr += s){
    uint64_t n_bit_errors = 0;
    uint64_t n_frame_errors = 0;
    uint64_t n_simu = 0;

    float snr_symb = snr + 10.0*log10f( ((float) K) / ((float) N) );
    float sigma = sqrt(1.0 / (2.0 * powf(10, snr_symb/10.0)));

    time_t begin_time, end_time;
    float total_time, avg_time;
  
    begin_time = time(NULL);
    do {
      uint8_t U_K[K];
      uint8_t C_N[N];
      int32_t X_N[N];
      float Y_N[N];
      float L_N[N];
      uint8_t V_K[K];
      // Transmitter
      source_generate(U_K, K);
      codec_repetition_encode(U_K, C_N, K, N/K);
      modem_BPSK_modulate(C_N, X_N, N);
      // Receiver
      channel_AWGN_add_noise(X_N, Y_N, N, sigma);
      modem_BPSK_demodulate(Y_N, L_N, N, sigma);
      codec_repetition_hard_decode(L_N, V_K, K, N/K);
      monitor_check_errors(U_K, V_K, K, &n_bit_errors, &n_frame_errors);
      // End
      n_simu++;
    } while(n_frame_errors < e);
    end_time = time(NULL);

    // total_time = (unsigned long) difftime(end_time, begin_time);
    total_time = end_time - begin_time;
    avg_time = total_time/ (float) n_simu; // average time for 1 frame
    // printf("%f/%f = %f\n", total_time, (float) n_simu, total_time / n_simu);
    printf("total_time = %f\n n_simu = %d\n avg_time = %f\n", total_time, n_simu, avg_time);
    // printf("total_time = %f");

    float ber = ((float) n_bit_errors) / (float) (n_simu * K);
    float fer = ((float) n_frame_errors) / (float) n_simu;
    printf("n_bit_error / (n_simu * K) = %f\n", ber);
    printf("n_frame_errors / n_simu = %f\n", fer);
    printf("\n");

    if(filecsv){
      fprintf(filecsv, "%f,%f,%f,%d,%d,%d,%f,%f,%f,%f\n",snr,snr_symb,sigma,n_bit_errors,n_frame_errors,n_simu,ber,fer,total_time,avg_time);
      printf("snr:%f\nsnr_symb:%f\nsigma:%f\nn_bit_errors:%d\nn_frame_errors:%d\nn_simu:%d\nber:%f\nfer:%f\ntotal_time:%f\navg_time:%f\n", snr,snr_symb,sigma,n_bit_errors,n_frame_errors,n_simu,ber,fer,total_time,avg_time);
    }
  }

  if (filecsv) fclose(filecsv);

  return 0;
}
