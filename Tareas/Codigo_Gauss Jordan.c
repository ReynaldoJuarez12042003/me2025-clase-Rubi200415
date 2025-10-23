#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int filas, columnas;
    
    printf("=== METODO DE GAUSS-JORDAN ===\n\n");
    
    // Leer dimensiones
    printf("Numero de ecuaciones: ");
    scanf("%d", &filas);
    printf("Numero de incognitas: ");
    scanf("%d", &columnas);
    
    // Reservar memoria
    double **A = (double **)malloc(filas * sizeof(double *));
    double *B = (double *)malloc(filas * sizeof(double));
    
    for(int i = 0; i < filas; i++) {
        A[i] = (double *)malloc(columnas * sizeof(double));
    }
    
    // Leer coeficientes
    printf("\nIngrese los coeficientes de la matriz A:\n");
    for(int i = 0; i < filas; i++) {
        printf("Ecuacion %d:\n", i+1);
        for(int j = 0; j < columnas; j++) {
            printf("  Coeficiente x%d: ", j+1);
            scanf("%lf", &A[i][j]);
        }
    }
    
    printf("\nIngrese los terminos independientes B:\n");
    for(int i = 0; i < filas; i++) {
        printf("Termino B%d: ", i+1);
        scanf("%lf", &B[i]);
    }
    
    // Mostrar sistema
    printf("\nSISTEMA INGRESADO:\n");
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            printf("%8.2f ", A[i][j]);
        }
        printf(" | %8.2f\n", B[i]);
    }
    
    // Método de Gauss-Jordan
    printf("\nAplicando Gauss-Jordan...\n");
    
    for(int i = 0; i < filas; i++) {
        // Pivoteo simple
        if(fabs(A[i][i]) < 1e-10) {
            for(int k = i+1; k < filas; k++) {
                if(fabs(A[k][i]) > 1e-10) {
                    // Intercambiar filas
                    double *tempA = A[i];
                    A[i] = A[k];
                    A[k] = tempA;
                    
                    double tempB = B[i];
                    B[i] = B[k];
                    B[k] = tempB;
                    printf("Intercambiado fila %d con fila %d\n", i+1, k+1);
                    break;
                }
            }
        }
        
        // Normalizar fila pivote
        double pivote = A[i][i];
        if(fabs(pivote) > 1e-10) {
            for(int j = 0; j < columnas; j++) {
                A[i][j] /= pivote;
            }
            B[i] /= pivote;
        }
        
        // Eliminar en otras filas
        for(int k = 0; k < filas; k++) {
            if(k != i && fabs(A[k][i]) > 1e-10) {
                double factor = A[k][i];
                for(int j = 0; j < columnas; j++) {
                    A[k][j] -= factor * A[i][j];
                }
                B[k] -= factor * B[i];
            }
        }
        
        // Mostrar paso intermedio
        printf("\nPaso %d:\n", i+1);
        for(int m = 0; m < filas; m++) {
            for(int j = 0; j < columnas; j++) {
                printf("%8.4f ", A[m][j]);
            }
            printf(" | %8.4f\n", B[m]);
        }
    }
    
    // Verificar solución
    printf("\nMATRIZ FINAL:\n");
    for(int i = 0; i < filas; i++) {
        for(int j = 0; j < columnas; j++) {
            printf("%8.4f ", A[i][j]);
        }
        printf(" | %8.4f\n", B[i]);
    }
    
    // Mostrar solución
    printf("\nSOLUCION:\n");
    int tieneSolucion = 1;
    
    for(int i = 0; i < filas; i++) {
        int filaCero = 1;
        for(int j = 0; j < columnas; j++) {
            if(fabs(A[i][j]) > 1e-10) {
                filaCero = 0;
                break;
            }
        }
        if(filaCero && fabs(B[i]) > 1e-10) {
            printf("Sistema incompatible (no tiene solucion)\n");
            tieneSolucion = 0;
            break;
        }
    }
    
    if(tieneSolucion) {
        for(int j = 0; j < columnas; j++) {
            // Buscar el 1 en la columna j
            int encontrado = 0;
            for(int i = 0; i < filas; i++) {
                if(fabs(A[i][j] - 1.0) < 1e-10) {
                    printf("x%d = %8.4f\n", j+1, B[i]);
                    encontrado = 1;
                    break;
                }
            }
            if(!encontrado) {
                printf("x%d = libre\n", j+1);
            }
        }
    }
    
    // Verificar
    if(tieneSolucion) {
        printf("\nVERIFICACION:\n");
        for(int i = 0; i < filas; i++) {
            double suma = 0;
            for(int j = 0; j < columnas; j++) {
                // Buscar valor de xj
                double xj = 0;
                for(int k = 0; k < filas; k++) {
                    if(fabs(A[k][j] - 1.0) < 1e-10) {
                        xj = B[k];
                        break;
                    }
                }
                suma += A[i][j] * xj;
            }
            printf("Ecuacion %d: %.4f = %.4f\n", i+1, suma, B[i]);
        }
    }
    
    // Liberar memoria
    for(int i = 0; i < filas; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    
    return 0;
}