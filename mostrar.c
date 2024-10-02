#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal_num) {
    printf("El proceso con PID %d recibió la señal %d\n", getpid(), signal_num);
}

int main() {
    signal(SIGINT, handle_signal);    // 2
    signal(SIGSTKFLT, handle_signal); // 16
    signal(SIGCHLD, handle_signal);   // 17
    signal(SIGCONT, handle_signal);    // 18
    printf("Proceso receptor iniciado. PID: %d\n", getpid());
    while (1) {
        pause(); 
    }

    return 0; 
}
