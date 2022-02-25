#include <stdio.h>
#include <time.h>

int main() {
	clock_t start, end;
	int i;
	
	start = clock();
	for (i = 0; i < 1000000; i++) {}
	for (i = 0; i < 1000000; i++) {}
	for (i = 0; i < 1000000; i++) {}
	end = clock();
	
	double period = (double)(end - start);
	printf("Este es el periodo %f\n", period);
	return 0;
}
