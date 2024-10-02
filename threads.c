#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

#define MESSAGE_KEY 1234

struct message_buffer {
    long message_type;
    int signal_number;
    int process_id;
};

void* handle_messages(void* arg) {
    int message_type = *(int*)arg; // Tipo de mensaje para este hilo
    int message_queue_id;
    struct message_buffer msg;

    message_queue_id = msgget(MESSAGE_KEY, 0666);
    if (message_queue_id < 0) {
        perror("Error al obtener la cola de mensajes");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (msgrcv(message_queue_id, &msg, sizeof(msg) - sizeof(long), message_type, 0) < 0) {
            perror("Error al recibir el mensaje");
            exit(EXIT_FAILURE);
        }

        printf("Hilo de tipo %d recibió: Señal %d para PID %d\n", message_type, msg.signal_number, msg.process_id);

        if (kill(msg.process_id, 0) == 0) {
            // Enviar la señal al proceso
            if (kill(msg.process_id, msg.signal_number) < 0) {
                perror("Error al enviar la señal");
            } else {
                printf("Señal %d enviada al proceso %d\n", msg.signal_number, msg.process_id);
            }
        } else {
            printf("El proceso con PID %d no existe o ya ha terminado.\n", msg.process_id);
        }
    }
    return NULL;
}

int main() {
    pthread_t thread_one, thread_two;
    int type_one = 1; // Para señales 2, 16, 17
    int type_two = 2; // Para señales 18, 19

    if (pthread_create(&thread_one, NULL, handle_messages, &type_one) != 0) {
        perror("Error al crear el primer hilo");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread_two, NULL, handle_messages, &type_two) != 0) {
        perror("Error al crear el segundo hilo");
        exit(EXIT_FAILURE);
    }

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    return 0;
}
