#include <stdio.h>

/* This program prints the Fibonacci numbers from F(2) through F(200). It is incorrect. */

int main () {
	long long int A = 1, B = 1;

	for (int i=3; i<=200; i++) {
		long long int C = A + B;
		printf ("F(%d) = %lld\n", i, C);
		A = B;
		B = C;
	}
	return 0;
}
