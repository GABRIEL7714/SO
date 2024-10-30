#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>

#define SHM_SIZE 64

int semid;
char *shm;

void p() {
    struct sembuf sb = {0, -1, 0};
    semop(semid, &sb, 1);
}

void v() {
    struct sembuf sb = {0, 1, 0};
    semop(semid, &sb, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <numero>\n", argv[0]);
        exit(1);
    }

    int param = atoi(argv[1]);
    int name = param;
    key_t key = 5678;
    int shmid = shmget(key, SHM_SIZE, 0666);
    semid = semget(key, 1, 0666);
    char *shm;

    if (shmid < 0 || semid < 0) {
        perror("Error al acceder share memory y semaphora");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    while (1) {
        p();

        int numero = atoi(shm);
//        printf("Share memory: %d%s\n", numero, shm + 3);

        if (numero == param - 1) {
            snprintf(shm, SHM_SIZE, "%03dholasoy%s%d", param, argv[0], name);
            printf("Escribiendo en la share memory: %s\n", shm);
            param +=3;
	    sleep(1);
        }

        v();


        if (param >= 999) {
            printf("Número máximo alcanzado: %d.\n", param);
            break;
        }
    }

    shmdt(shm);
    return 0;
}
