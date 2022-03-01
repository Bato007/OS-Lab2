#include <stdio.h>
#include <time.h>

int main() {
	clock_t start, end;
	int i;
	
	start = clock();
	for (i = 0; i < 1000000; i++) {
		printf("%d", i);
	}
	for (i = 0; i < 1000000; i++) {
		printf("%d", i);
	}
	for (i = 0; i < 1000000; i++) {
		printf("%d", i);
	}
	end = clock();
	
	double period = (double)(end - start);
	printf("\nEste es el periodo %f\n", period);
	printf("El tiempoo en segunds es: %f\n", (period / CLOCKS_PER_SEC));
	return 0;
}
