#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2], pipefd2[2];
    pid_t pid;
    FILE* entrada = NULL;
    FILE* salida1 = NULL;
    FILE* salida2 = NULL;
    int contador = 0;
    char linea[100];

    // Crear los pipes
    if (pipe(pipefd) == -1 || pipe(pipefd2) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        close(pipefd[0]);  
        close(pipefd2[1]); 

        entrada = fopen("entrada.txt", "r");
        if (entrada == NULL)
        {
            perror("Error al abrir entrada");
            exit(EXIT_FAILURE);
        }

        while (fgets(linea, sizeof(linea), entrada) != NULL)
        {
            write(pipefd[1], linea, strlen(linea));
        }

        fclose(entrada);
        close(pipefd[1]);  

        int line_count;
        read(pipefd2[0], &line_count, sizeof(int)); 

        salida2 = fopen("salida2.txt", "w");
        if (salida2 == NULL)
        {
            perror("Error al abrir salida 2");
            exit(EXIT_FAILURE);
        }
        fprintf(salida2, "Líneas escritas: %d\n", line_count);  
        fclose(salida2);

        close(pipefd2[0]);  
    }
    else 
    {
        close(pipefd[1]);  
        close(pipefd2[0]); 

        salida1 = fopen("salida1.txt", "w");
        if (salida1 == NULL)
        {
            perror("Error al abrir salida 1");
            exit(EXIT_FAILURE);
        }

        while (read(pipefd[0], linea, sizeof(linea)) > 0)
        {
            fputs(linea, salida1);
            contador++;
            printf("%s", linea); 
        }

        fclose(salida1);
        close(pipefd[0]);  

        write(pipefd2[1], &contador, sizeof(int));
        close(pipefd2[1]);  

        wait(NULL);
    }

    return 0;
}
