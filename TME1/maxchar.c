#include <stdio.h>

int main(){
	char c = 0;
    int n = 0;
    short short_n = 0;
    unsigned char u_n = 0;

    // char
    printf("char type:\n");
	for (char i = 0; i >= 0; i--){ c = i; }
	printf("vmin = %d\n", c);
	for (char i = 0; i >= 0; i++){ c = i; }
	printf("vmax = %d\n", c);

    // unsigned char
    printf("\nunsigned char type:\n");
    for (unsigned char i = 1; i != 0; i--){ u_n = i; }
	printf("vmin = %d\n", u_n);
	for (unsigned char i = 1; i != 0; i++){ u_n = i; }
	printf("vmax = %d\n", u_n);

    // int
    printf("\nint type:\n");
    for (int i = 0; i >= 0; i--){ n = i; }
	printf("vmin = %d\n", n);
	for (int i = 0; i >= 0; i++){ n = i; }
	printf("vmax = %d\n", n);

    // short 
    printf("\nshort type:\n");
    for (short i = 0; i >= 0; i--){ short_n = i; }
	printf("vmin = %d\n", short_n);
	for (short i = 0; i >= 0; i++){ short_n = i; }
	printf("vmax = %d\n", short_n);

	return 0;
}
