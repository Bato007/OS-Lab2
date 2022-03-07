#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

#define PAGE_SIZE 1024
#define READ 0
#define WRITE 1

int main(int argc, char** argv) {
  if (argc != 3) { 
    printf("El numero de parametros es incorrecto\n");
    return -1;
  }

  // Gets the arguments and checks if correct format
  int n = atoi(argv[1]);
  char* shared_memory;
  char* temp_x = argv[2];
  char x = temp_x[0];
  int i, errnum, fd, pipe_parent[2], pipe_son[2];
	pid_t son;

  // Creates the shared memory object
  fd = shm_open("/memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
  if (fd == -1) { // Checks error
    fprintf(stderr, "Fallo al crear el file descriptor\n");
    return -1;
  }

  // Truncating memory
  int rc = ftruncate(fd, PAGE_SIZE);
  if (rc == -1) {
    fprintf(stderr, "Fallo al extender la memoria\n");
  }

  // Now opens a mmap
  shared_memory = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (shared_memory == MAP_FAILED) { // Checks error
    fprintf(stderr, "Fallo al crear la memoria compartida\n");
    return -1;
  }

  printf("Memoria compartida: %p\n", shared_memory);

  // Creando los pipes
  if (pipe(pipe_parent) == -1 || pipe(pipe_son) == -1) {
    fprintf(stderr, "Fallo al crear el medio de comunicacion\n");
    return -1;
  }

  // Creando al hijo
  son = fork();
  if (son < 0) {
    fprintf(stderr, "Fallo al crear al hijo\n");
    return -1;
  } 
  
  if (son == 0) {
    int son_value = 0;
    int to_dad = 0;

    // Se suman indices para encontrar en dnde escribir
    while (1) {
      if (shared_memory[to_dad] == NULL) {
        break;
      }
      to_dad++;
    }


    // Cerrar pipes que no se usan
    close(pipe_parent[WRITE]);
    close(pipe_son[READ]);

    while (1) {
      if (read(pipe_parent[READ], &son_value, sizeof(son_value)) > 0) {
        shared_memory[to_dad] = x;
        to_dad++;
      } else {
        break;
      }
      write(pipe_son[WRITE], &to_dad, sizeof(to_dad));
    }

    // Cerrando pipes al finalizar
    close(pipe_parent[READ]);
    close(pipe_son[WRITE]);

  } else {
    // Cerrar pipes que no se usan
    int parent_value = 1;
    int from_son = 0;
    int current_pos = 0;
    close(pipe_parent[READ]);
    close(pipe_son[WRITE]);

    for (i = 0; i < PAGE_SIZE; i++) {
      if ((i % n) == 0) {
        // Le dice al hijo que tiene que escribir
        write(pipe_parent[WRITE], &parent_value, sizeof(parent_value));

        if (read(pipe_son[READ], &from_son, sizeof(int)) > 0) {
          parent_value++;
        }
      }
    }

    // Esperamos al hijo
    write(pipe_parent[WRITE], 0, sizeof(int));
    wait(NULL);

    // Cerrando pipes al finalizar
    close(pipe_parent[WRITE]);
    close(pipe_son[READ]);

    // Se muestra la informacion
    printf("Se muestra lo que esta escrito en memoria:\n");
    while (1) {
      if (shared_memory[current_pos] == NULL) {
        break;
      }
      printf("%c", shared_memory[current_pos]);
    }
    printf("\n");
  }

  return 0;
}