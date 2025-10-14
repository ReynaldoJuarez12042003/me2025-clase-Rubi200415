#include <stdio.h>
#include <math.h>

#define TAM 3

int main() {
    int fila, col, iter;
    float sistema[TAM][TAM+1] = {
        {2, 3, 0, 8},
        {4, 6, 7, -3},
        {2, 1, 6, 5}
    };
    float temp[TAM+1], resultado[TAM];

    printf("=== Resolucion de Sistemas Lineales ===\n\n");
    printf("Matriz inicial:\n");
    for (fila = 0; fila < TAM; fila++) {
        for (col = 0; col < TAM + 1; col++) {
            printf("%7.2f ", sistema[fila][col]);
        }
        printf("\n");
    }

    // Proceso de triangularización
    for (iter = 0; iter < TAM - 1; iter++) {
        // Selección del elemento principal
        int fila_principal = iter;
        float valor_max = fabs(sistema[iter][iter]);
        for (fila = iter + 1; fila < TAM; fila++) {
            if (fabs(sistema[fila][iter]) > valor_max) {
                valor_max = fabs(sistema[fila][iter]);
                fila_principal = fila;
            }
        }

        // Intercambio de ecuaciones si es necesario
        if (fila_principal != iter) {
            for (col = 0; col < TAM + 1; col++) {
                temp[col] = sistema[iter][col];
                sistema[iter][col] = sistema[fila_principal][col];
                sistema[fila_principal][col] = temp[col];
            }
        }

        // Verificación de pivote válido
        if (fabs(sistema[iter][iter]) < 1e-7) {
            printf("\nSistema sin solucion unica en paso %d.\n", iter + 1);
            return 1;
        }

        // Reducción del sistema
        for (fila = iter + 1; fila < TAM; fila++) {
            float coef = sistema[fila][iter] / sistema[iter][iter];
            for (col = iter; col < TAM + 1; col++) {
                sistema[fila][col] -= coef * sistema[iter][col];
            }
        }
    }

    // Obtención de la solución
    for (fila = TAM - 1; fila >= 0; fila--) {
        float acum = 0.0;
        for (col = fila + 1; col < TAM; col++) {
            acum += sistema[fila][col] * resultado[col];
        }
        resultado[fila] = (sistema[fila][TAM] - acum) / sistema[fila][fila];
    }

    printf("\nMatriz triangular resultante:\n");
    for (fila = 0; fila < TAM; fila++) {
        for (col = 0; col < TAM + 1; col++) {
            printf("%7.2f ", sistema[fila][col]);
        }
        printf("\n");
    }

    printf("\nVariables de la solucion:\n");
    for (fila = 0; fila < TAM; fila++) {
        printf("Incognita %d = %6.3f\n", fila + 1, resultado[fila]);
    }

    return 0;
}