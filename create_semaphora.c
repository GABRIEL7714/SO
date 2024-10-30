#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 64

int main() {
    key_t key = 5678;
    int shmid = shmget(key, SHM_SIZE, 0666);
    char *shm;
    if (shmid < 0) {
        perror("ErrorShare Memory");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("Error al enlazar memoria compartida");
        exit(1);
    }

    while (1) {
        printf("Contenido: %s\n", shm);
        sleep(1);
    }

    shmdt(shm);
    return 0;
}
