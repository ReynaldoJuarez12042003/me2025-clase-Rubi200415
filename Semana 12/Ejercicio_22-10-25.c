#include <stdio.h>
#include <stdlib.h>

int main() {
    int n = 3;
    int i, j, k;
    
    printf("SISTEMA DE REACTORES - FACTORIZACION LU\n\n");
    
    // Matriz A
    double A[3][3] = {
        {15, -3, -1},
        {-4, -1, 12},
        {-3, 18, -6}
    };
    
    // Vector B
    double B[3] = {3800, 2350, 1200};
    
    // Matrices L y U
    double L[3][3] = {0};
    double U[3][3] = {0};
    
    // Vectores solución
    double Y[3] = {0};
    double X[3] = {0};
    
    // Mostrar sistema
    printf("Sistema:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            printf("%6.1f ", A[i][j]);
        }
        printf(" | %6.1f\n", B[i]);
    }
    
    // Factorización LU
    for(i = 0; i < n; i++) {
        L[i][i] = 1;
        
        for(j = i; j < n; j++) {
            double sum = 0;
            for(k = 0; k < i; k++) {
                sum += L[i][k] * U[k][j];
            }
            U[i][j] = A[i][j] - sum;
        }
        
        for(j = i + 1; j < n; j++) {
            double sum = 0;
            for(k = 0; k < i; k++) {
                sum += L[j][k] * U[k][i];
            }
            L[j][i] = (A[j][i] - sum) / U[i][i];
        }
    }
    
    // Resolver LY = B
    for(i = 0; i < n; i++) {
        Y[i] = B[i];
        for(j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
    }
    
    // Resolver UX = Y
    for(i = n-1; i >= 0; i--) {
        X[i] = Y[i];
        for(j = i + 1; j < n; j++) {
            X[i] -= U[i][j] * X[j];
        }
        X[i] /= U[i][i];
    }
    
    // Mostrar resultados
    printf("\nSOLUCION:\n");
    printf("X1 = %.2f mg/m3\n", X[0]);
    printf("X2 = %.2f mg/m3\n", X[1]);
    printf("X3 = %.2f mg/m3\n", X[2]);
    
    // Verificar
    printf("\nVERIFICACION:\n");
    for(i = 0; i < n; i++) {
        double sum = 0;
        for(j = 0; j < n; j++) {
            sum += A[i][j] * X[j];
        }
        printf("Ec.%d: %.1f = %.1f\n", i+1, sum, B[i]);
    }
    
    return 0;
}