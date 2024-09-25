#include <stdio.h>

int main() {
    char buffer[256];

    // Leer desde el input estándar hasta EOF (Ctrl+D en la terminal)
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Escribir en el output estándar
        fputs(buffer, stdout);
    }

    return 0;
}
