#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <time.h>      /* pour _SC... */
#include <sys/times.h> /* pour times */
#include <unistd.h>    /* pour sysconf */

#include "transmitter.h"
#include "receiver.h"

int main(int argc, char** argv){
  float m = 0;
  float M = 12;
  float s = 1;
  uint e = 100;
  uint K = 8;
  uint N = 32;
  char D[15] = "rep-hard";
  FILE* filecsv = NULL;

  // Options handling
  char opt;
  int src_all_zeros = 0;
  int mod_all_ones = 0;
  int opt_index = 0;
  static struct option long_options[] = {
    {"src-all-zeros", no_argument, 0, '0'},
    {"mod-all-ones", no_argument, 0, '1'},
    {0,   0,    0,    0}
  };

  while((opt=getopt_long(argc, argv, "m:M:s:e:K:N:D:o:E", long_options, &opt_index)) != -1){
    switch(opt){
      // case 0:
      //   if (!strcmp(long_options[opt_index].name, "src-all-zeros")){
      //     src_all_zeros = 1;
      //   }
      //   else if (!strcmp(long_options[opt_index].name, "mod-all-ones")){
      //     mod_all_ones = 1;
      //   } 
      //   break;
      case '?':
        break;
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
          fprintf(filecsv, "Eb/N0,Es/N0,sigma,be,fe,fn,BER,FER,Time,AverageTime\n");
        }
        break;
      case '0':
        printf("ALL ZEROS\n");
        src_all_zeros = 1;
        break;
      case '1':
        printf("ALL ONES\n");
        mod_all_ones = 1;
        break;
      default:;
    }
  }

  // Begin simulation
  srand(time(NULL));
  for (float snr = m; snr < M; snr += s){
    // Calculating 
    float snr_symb = snr + 10.0*log10f( ((float) K) / ((float) N) );
    float sigma = sqrt(1.0 / (2.0 * powf(10, snr_symb/10.0)));

    // Measures declarations
    float sim_thr;
    uint64_t n_bit_errors = 0;
    uint64_t n_frame_errors = 0;
    uint64_t n_simu = 0;

    // TIME HANDLER
    float ratio=1000 / sysconf(_SC_CLK_TCK); // to convert result from times() fonction in milliseconds
    /* 0: begin frame, 1: src generate, 2: encode, 3: modulate, 4: channel, 5: demodulate, 6: decode, 7: monitor */
    struct tms time_steps[8];
    /* 0: frame, 1: src generate, 2: encode, 3: modulate, 4: channel, 5: demodulate, 6: decode, 7: monitor */
    float total_duration[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    float min_duration[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    float max_duration[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    float avg_duration[8];
    float percent_duration[8];
    float throughput[8];
    float temp_duration;

    // SIMULATION
    do {
      uint8_t U_K[K];
      uint8_t C_N[N];
      int32_t X_N[N];
      float Y_N[N];
      float L_N[N];
      uint8_t V_K[K];

      times(&time_steps[0]); // frame begin step

      // Transmitter
      if (src_all_zeros) source_generate_all_zeros(U_K, K);
      else source_generate(U_K, K);
      times(&time_steps[1]);

      codec_repetition_encode(U_K, C_N, K, N/K);
      times(&time_steps[2]);

      if (mod_all_ones) modem_BPSK_modulate_all_ones(C_N, X_N, N);
      else modem_BPSK_modulate(C_N, X_N, N);
      times(&time_steps[3]);

      // Channel
      channel_AWGN_add_noise(X_N, Y_N, N, sigma);
      times(&time_steps[4]);

      // Receiver
      modem_BPSK_demodulate(Y_N, L_N, N, sigma);
      times(&time_steps[5]);

      codec_repetition_hard_decode(L_N, V_K, K, N/K);
      times(&time_steps[6]);

      monitor_check_errors(U_K, V_K, K, &n_bit_errors, &n_frame_errors);
      times(&time_steps[7]); // frame end step

      // End
      n_simu++;

      // Total duration measure
      temp_duration = ratio * (time_steps[7].tms_utime - time_steps[0].tms_utime);
      if (max_duration[0] < temp_duration || max_duration[0] == -1) max_duration[0] = temp_duration; // max
      if (min_duration[0] > temp_duration)                          min_duration[0] = temp_duration; // min
      total_duration[0] += temp_duration; // duration of the frame

      // Additionnal measures
      #ifdef ENABLE_STATS
      for (int i = 1; i < 8; i++){
        temp_duration = ratio * (time_steps[i].tms_utime - time_steps[i-1].tms_utime);
        if (max_duration[i] < temp_duration || max_duration[i] == -1) max_duration[i] = temp_duration; // max
        if (min_duration[i] > temp_duration)                          min_duration[i] = temp_duration; // min
        total_duration[i] += temp_duration; // duration of the step i
      }
      #endif
    } while(n_frame_errors < e);

    // Global measures
    float ber = ((float) n_bit_errors) / (float) (n_simu * K);
    float fer = ((float) n_frame_errors) / (float) n_simu;  
    avg_duration[0] = total_duration[0] / (float) n_simu;
    throughput[0] = (((float) (n_simu * K)) / total_duration[0]) * 1e-3; // throughput in megabits per seconds (nb bits transferred / total duration of simulation)

    // Additionnal measures
    #ifdef ENABLE_STATS
    for (int i = 1; i < 8; i++){
      avg_duration[i] = total_duration[i] / (float) n_simu;
    }
    for (int i = 0; i < 8; i++){
      percent_duration[i] = (total_duration[i] / total_duration[0]) * 100.0;
    }
    for (int i = 1; i < 8; i++){
      switch(i){
        case 0:case 1:case 6:case 7:
          throughput[i] = (((float) (n_simu * K)) / total_duration[i]) * 1e-3;
          break;
        case 2:case 3:case 4:case 5:
          throughput[i] = (((float) (n_simu * N)) / total_duration[i]) * 1e-3;
          break;
      }
    }
    #endif

    // Writing csv line
    if(filecsv){
      if(fprintf(filecsv, "%f,%f,%f,%d,%d,%d,%f,%f,%f,%f\n",snr,snr_symb,sigma,n_bit_errors,n_frame_errors,n_simu,ber,fer,total_duration[0],avg_duration[0])){
        printf("succesfull writing frame for SNR %f\n", snr);
      }
    }
 
    // Printing Statistics
    printf("\n----- Statistics for SNR=%f -----\n", snr);
    printf("-- Total time --\n");
    printf("Number of bits transfered: %d bits\n", K*n_simu);
    printf("Total duration: %f ms\n", total_duration[0]);
    printf("Average duration: %f ms\n", avg_duration[0]);
    printf("Maximal duration: %f ms\n", max_duration[0]);
    printf("Minimal duration: %f ms\n", min_duration[0]);
    printf("Throughput of the communication: %f mbps\n", throughput[0]);
    printf("\n");

    // Additionnal Statistics (compile with -DENABLE_STATS to print them, "make stats")
    #ifdef ENABLE_STATS
    printf("-- Source generate --\n");
    printf("Total duration: %f ms\n", total_duration[1]);
    printf("Average duration: %f ms\n", avg_duration[1]);
    printf("Maximal duration: %f ms\n", max_duration[1]);
    printf("Minimal duration: %f ms\n", min_duration[1]);
    printf("Percentage of duration: %f %%\n", percent_duration[1]);
    printf("Throughput: %f mbps\n", throughput[1]);
    printf("\n");

    printf("-- Encoder --\n");
    printf("Total duration: %f ms\n", total_duration[2]);
    printf("Average duration: %f ms\n", avg_duration[2]);
    printf("Maximal duration: %f ms\n", max_duration[2]);
    printf("Minimal duration: %f ms\n", min_duration[2]);
    printf("Percentage of duration: %f %%\n", percent_duration[2]);
    printf("Throughput: %f mbps\n", throughput[2]);
    printf("\n");

    printf("-- Modulator --\n");
    printf("Total duration: %f ms\n", total_duration[3]);
    printf("Average duration: %f ms\n", avg_duration[3]);
    printf("Maximal duration: %f ms\n", max_duration[3]);
    printf("Minimal duration: %f ms\n", min_duration[3]);
    printf("Percentage of duration: %f %%\n", percent_duration[3]);
    printf("Throughput: %f mbps\n", throughput[3]);
    printf("\n");

    printf("-- Channel --\n");
    printf("Total duration: %f ms\n", total_duration[4]);
    printf("Average duration: %f ms\n", avg_duration[4]);
    printf("Maximal duration: %f ms\n", max_duration[4]);
    printf("Minimal duration: %f ms\n", min_duration[4]);
    printf("Percentage of duration: %f %%\n", percent_duration[4]);
    printf("Throughput: %f mbps\n", throughput[4]);
    printf("\n");

    printf("-- Demodulator --\n");
    printf("Total duration: %f ms\n", total_duration[5]);
    printf("Average duration: %f ms\n", avg_duration[5]);
    printf("Maximal duration: %f ms\n", max_duration[5]);
    printf("Minimal duration: %f ms\n", min_duration[5]);
    printf("Percentage of duration: %f %%\n", percent_duration[5]);
    printf("Throughput: %f mbps\n", throughput[5]);
    printf("\n");

    printf("-- Decoder --\n");
    printf("Total duration: %f ms\n", total_duration[6]);
    printf("Average duration: %f ms\n", avg_duration[6]);
    printf("Maximal duration: %f ms\n", max_duration[6]);
    printf("Minimal duration: %f ms\n", min_duration[6]);
    printf("Percentage of duration: %f %%\n", percent_duration[6]);
    printf("Throughput: %f mbps\n", throughput[6]);
    printf("\n");

    printf("-- Monitor --\n");
    printf("Total duration: %f ms\n", total_duration[7]);
    printf("Average duration: %f ms\n", avg_duration[7]);
    printf("Maximal duration: %f ms\n", max_duration[7]);
    printf("Minimal duration: %f ms\n", min_duration[7]);
    printf("Percentage of duration: %f %%\n", percent_duration[7]);
    printf("Throughput: %f mbps\n", throughput[7]);
    printf("\n");
    #endif
  }

  if (filecsv) fclose(filecsv);

  return 0;
}