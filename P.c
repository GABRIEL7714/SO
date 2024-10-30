#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>

#define SHM_SIZE 64

int main() {
    key_t key = 5678;
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    int semid = semget(key, 1, IPC_CREAT | 0666);
    char *shm;
    if (shmid < 0 || semid < 0) {
        perror("Error al crear.");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("Error al enlazar la share memory");
        exit(1);
    }

    strcpy(shm, "003holaucsp");

    semctl(semid, 0, SETVAL, 1);

    printf("Share Memory y semáforo inicializados con éxito.\n");

    shmdt(shm);
    return 0;
}
