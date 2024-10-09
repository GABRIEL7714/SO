#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h> // Agregado para usar la función sleep()

#define KEY 0x1111

union semun {
    int val;                  
    struct semid_ds *buf;    
    unsigned short *array;    
};

int main() {
    // Crear un conjunto de semáforos
    int sem_id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (sem_id < 0) {
        perror("Error al crear semáforo (semget)");
        exit(EXIT_FAILURE);
    }

    // Inicializar el semáforo a 1
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) < 0) {
        perror("Error al establecer valor del semáforo (semctl)");
        exit(EXIT_FAILURE);
    }

    printf("Semáforo creado y listo para ser utilizado.\n");

    // Bucle infinito para mantener el programa en ejecución
    while (1) {
        sleep(10);
    }

    return 0;
}
