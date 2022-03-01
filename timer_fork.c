#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t son, grandson, greatGrandson;
	clock_t start, end;
	int i, iterations = 1000000;
	
	// Start of program
	start = clock();
	son = fork();
	
	if (son == 0) {
		grandson = fork();
		if (grandson == 0) {
			
			greatGrandson = fork();
			if (greatGrandson == 0) {
				for (i = 0; i < iterations; i++) {
					printf("%d", i);
				}
			} else {
				for (i = 0; i < iterations; i++) {
					printf("%d", i);
				}
				wait(NULL);
			}

		} else {
			for (i = 0; i < iterations; i++) {
				printf("%d", i);
			}
			wait(NULL);
		}

	} else {
		wait(NULL);
		end = clock();
		printf("EL tiempop que le toma es el siguiente: %f\n", (double)(end - start));

	}
	return 0;
}
