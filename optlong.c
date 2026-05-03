#include <stdio.h>
#include <getopt.h>
#include <time.h>      /* pour _SC... */
#include <sys/times.h> /* pour times */
#include <unistd.h>    /* pour sysconf */

int main(int argc, char* const* argv){
  char c;

  while (1){
    int opt_index = 0;
    static struct option long_options[] = {
      {"booo", no_argument, 0, 0},
      {"toto", no_argument, 0, 0},
      {0,   0,    0,    0}
    };

    c = getopt_long(argc, argv, "abc", long_options, &opt_index);

    printf("optind = %d\n", optind);
    printf("opt_index = %d\n", opt_index);

    if (c == -1) {
      printf("End getopt_long\n");
      break;
    }

    switch(c){
      case 0:   printf("0: name of option: %s\n", long_options[opt_index].name);
                break;
      case '?': printf("?\n");
                break;
      default:
                printf("c = %c\n", c);
                break;
    }
  }

  // #ifdef OK
  // printf("OK\n");
  // #endif

  // float begin, end;
  // struct tms t1, t2;
  // double ratio=1000 / sysconf(_SC_CLK_TCK); /* peut etre flotant */

  // begin = times(&t1);
  // for (int i = 0; i < 1000000000; i++){
  //   if (i%100000000 == 0) printf("i = %d\n", i);
  // }
  // end = times(&t2);

  // printf("temps écoulé: %f et begin = %d, end = %d\n", ratio * (t2.tms_utime - t1.tms_utime), begin, end);
  return 0;
}