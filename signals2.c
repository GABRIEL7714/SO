#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h> // Incluir esta biblioteca para getpid

#define KEY 1234

struct message {
    long mtype;
    int signal_number;
    int process_id;
};

void show_menu() {
    printf("\nSeleccione una señal a enviar:\n");
    printf("1. SIGINT (Señal 2)\n");
    printf("2. SIGSTKFLT (Señal 16)\n");
    printf("3. SIGCHLD (Señal 17)\n");
    printf("4. SIGCONT (Señal 18)\n");
    printf("5. SIGSTOP (Señal 19)\n");
    printf("6. Salir\n");
    printf("Opción: ");
}

int select_signal(int option) {
    switch (option) {
        case 1: return SIGINT;
        case 2: return SIGSTKFLT;
        case 3: return SIGCHLD;
        case 4: return SIGCONT;
        case 5: return SIGSTOP;
        default: return -1; 
    }
}

int main() {
    int msg_queue_id;
    struct message msg;
    int choice;
    pid_t process_id = getpid(); // Obtener el PID del proceso actual

    if ((msg_queue_id = msgget(KEY, IPC_CREAT | 0666)) == -1) {
        perror("Error creando la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    while (1) {
        show_menu();
        scanf("%d", &choice);

        if (choice == 6) break;

        int signal = select_signal(choice);
        if (signal == -1) {
            printf("Opción no válida. Intente de nuevo.\n");
            continue;
        }

        // Aquí se usa el PID del proceso actual en lugar de pedirlo al usuario
        msg.mtype = (choice <= 3) ? 1 : 2;
        msg.signal_number = signal;
        msg.process_id = process_id; // Usar el PID obtenido automáticamente

        if (msgsnd(msg_queue_id, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("Error enviando el mensaje");
            exit(EXIT_FAILURE);
        }

        printf("Mensaje enviado: Señal %d enviada al proceso con PID %d\n", msg.signal_number, msg.process_id);
    }

    return 0;
}
