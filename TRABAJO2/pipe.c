#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int pipefd[2]; 
    pid_t pid;
    char buffer[1024];
    char *source_file = "fuente.txt";   // archivo fuente con los mensajes
    char *output_file = "output.txt";   // archivo donde se almacenarán los mensajes del hijo
    char *count_file = "count.txt";     // archivo donde se almacenará la cantidad de mensajes

    // Crear la pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Crear el proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // PROCESO HIJO
        close(pipefd[0]); // Cerrar el extremo de lectura del pipe

        // Abrir el archivo fuente para leer
        int src_fd = open(source_file, O_RDONLY);
        if (src_fd == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }

        // Leer el contenido del archivo y escribirlo en el pipe
        ssize_t bytes_read;
        while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
            write(pipefd[1], buffer, bytes_read);
        }

        close(src_fd);   // Cerrar el archivo fuente
        close(pipefd[1]); // Cerrar el extremo de escritura del pipe
    } else {
        // PROCESO PADRE
        close(pipefd[1]); // Cerrar el extremo de escritura del pipe

        // Abrir el archivo de salida para escribir los mensajes del hijo
        int out_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (out_fd == -1) {
            perror("open output file");
            exit(EXIT_FAILURE);
        }

        // Abrir o crear el archivo para contar los mensajes recibidos
        int count_fd = open(count_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (count_fd == -1) {
            perror("open count file");
            exit(EXIT_FAILURE);
        }

        // Inicializar el contador de mensajes
        int message_count = 0;
        ssize_t bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            write(out_fd, buffer, bytes_read); // Escribir los datos recibidos en el archivo de salida
            message_count++;  // Incrementar el contador de mensajes
        }

        // Escribir el número total de mensajes en el archivo count.txt
        char count_buffer[100];
        sprintf(count_buffer, "Cantidad de mensajes recibidos: %d\n", message_count);
        write(count_fd, count_buffer, strlen(count_buffer));

        // Cerrar los archivos y el pipe
        close(out_fd);   // Cerrar el archivo de salida
        close(count_fd); // Cerrar el archivo de contador
        close(pipefd[0]); // Cerrar el extremo de lectura del pipe
    }

    return 0;
}
