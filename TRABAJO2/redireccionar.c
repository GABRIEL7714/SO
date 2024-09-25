#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int fd_read, fd_write;

    // Archivos de redirección
    char *input_file = "input.txt";   // Archivo de entrada
    char *output_file = "output.txt"; // Archivo de salida

    // Abrir el archivo de entrada en modo lectura
    fd_read = open(input_file, O_RDONLY);
    if (fd_read < 0) {
        perror("Error abriendo archivo de entrada");
        exit(EXIT_FAILURE);
    }

    // Abrir el archivo de salida en modo escritura (crea si no existe)
    fd_write = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_write < 0) {
        perror("Error abriendo archivo de salida");
        exit(EXIT_FAILURE);
    }

    // Crear un proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        // Redirigir la entrada estándar al archivo de entrada
        if (dup2(fd_read, STDIN_FILENO) < 0) {
            perror("Error en dup2 para la entrada");
            exit(EXIT_FAILURE);
        }
        close(fd_read);  // Cerrar el descriptor de archivo, ya no es necesario

        // Redirigir la salida estándar al archivo de salida
        if (dup2(fd_write, STDOUT_FILENO) < 0) {
            perror("Error en dup2 para la salida");
            exit(EXIT_FAILURE);
        }
        close(fd_write);  // Cerrar el descriptor de archivo, ya no es necesario

        // Ejecutar el primer programa que lee de stdin y escribe en stdout
        execlp("./programa_lectura_escritura", "programa_lectura_escritura", NULL);

        // Si execlp falla
        perror("Error ejecutando el programa");
        exit(EXIT_FAILURE);
    } else {  // Proceso padre
        // Esperar a que el proceso hijo termine
        wait(NULL);
        printf("El proceso hijo ha terminado.\n");
    }

    return 0;
}
