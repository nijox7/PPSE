#include <stdio.h>

int main() {
    double s = 1;

    for (int n = 1; n <= 128; n++){
        s = 2*s + 1;
        if (((int)s)%2 == 0){
            printf("s_%d = %f\n", n, s);
        }
    }

    return 0;
}