#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	pid_t son;
	
	// Start of program
	son = fork();

  if (son == 0) {
    for (int i = 0; i < 4000000; i++) {
      printf("%d ", i);
    }
  } else {
    while(1) {}
  }
	
	return 0;
}