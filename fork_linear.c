#include <stdio.h>
#include <unistd.h>

int main() {
	fork();
	fork();
	fork();
	fork();
	printf("Por aqui paso un proceso\n");
	return 0;
}
