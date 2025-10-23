#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int m, n;
    
    printf("RESOLUCION DE SISTEMAS DE ECUACIONES\n");
    printf("Metodo de Gauss-Seidel para Matrices Rectangulares\n\n");
    
    printf("Numero de ecuaciones (m): ");
    scanf("%d", &m);
    printf("Numero de incognitas (n): ");
    scanf("%d", &n);
    
    // Crear matrices
    double **A = (double **)malloc(m * sizeof(double *));
    double **ATA = (double **)malloc(n * sizeof(double *));
    double *B = (double *)malloc(m * sizeof(double));
    double *ATB = (double *)malloc(n * sizeof(double));
    double *X = (double *)malloc(n * sizeof(double));
    double *X_old = (double *)malloc(n * sizeof(double));
    
    for(int i = 0; i < m; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
    }
    for(int i = 0; i < n; i++) {
        ATA[i] = (double *)malloc(n * sizeof(double));
    }
    
    // Leer matriz A
    printf("\nIngrese la matriz A (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        printf("Ecuacion %d:\n", i+1);
        for(int j = 0; j < n; j++) {
            printf("  Coeficiente x%d: ", j+1);
            scanf("%lf", &A[i][j]);
        }
    }
    
    // Leer vector B
    printf("\nIngrese el vector B:\n");
    for(int i = 0; i < m; i++) {
        printf("B[%d]: ", i+1);
        scanf("%lf", &B[i]);
    }
    
    // Leer valores iniciales
    printf("\nIngrese valores iniciales de X:\n");
    for(int i = 0; i < n; i++) {
        printf("X%d[0]: ", i+1);
        scanf("%lf", &X[i]);
        X_old[i] = X[i];
    }
    
    // Leer criterios de parada
    int max_iter;
    double tolerancia;
    printf("\nNumero maximo de iteraciones: ");
    scanf("%d", &max_iter);
    printf("Tolerancia (ej. 0.0001): ");
    scanf("%lf", &tolerancia);
    
    // Mostrar sistema
    printf("\nSISTEMA ORIGINAL (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.2f", A[i][j]);
        }
        printf("  |%8.2f\n", B[i]);
    }
    
    // Para matrices rectangulares, usar A^T * A * X = A^T * B
    printf("\nCalculando A^T * A y A^T * B...\n");
    
    // Calcular A^T * A (n x n)
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            ATA[i][j] = 0.0;
            for(int k = 0; k < m; k++) {
                ATA[i][j] += A[k][i] * A[k][j];
            }
        }
    }
    
    // Calcular A^T * B (n x 1)
    for(int i = 0; i < n; i++) {
        ATB[i] = 0.0;
        for(int k = 0; k < m; k++) {
            ATB[i] += A[k][i] * B[k];
        }
    }
    
    // Mostrar sistema normal
    printf("\nSISTEMA NORMAL (A^T * A * X = A^T * B) (%dx%d):\n", n, n);
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.2f", ATA[i][j]);
        }
        printf("  |%8.2f\n", ATB[i]);
    }
    
    // METODO DE GAUSS-SEIDEL
    printf("\nMETODO DE GAUSS-SEIDEL:\n");
    printf("Iteracion 0: ");
    for(int i = 0; i < n; i++) {
        printf("X%d=%.4f ", i+1, X[i]);
    }
    printf("\n");
    
    int iter;
    double error = tolerancia + 1;
    
    for(iter = 1; iter <= max_iter && error > tolerancia; iter++) {
        error = 0.0;
        
        // Guardar valores anteriores
        for(int i = 0; i < n; i++) {
            X_old[i] = X[i];
        }
        
        // Aplicar Gauss-Seidel al sistema normal
        for(int i = 0; i < n; i++) {
            double suma = 0.0;
            
            // Suma de ATA[i][j] * X[j] para j != i
            for(int j = 0; j < n; j++) {
                if(j != i) {
                    suma += ATA[i][j] * X[j];
                }
            }
            
            // Actualizar X[i] usando los valores más recientes
            if(fabs(ATA[i][i]) > 1e-10) {
                X[i] = (ATB[i] - suma) / ATA[i][i];
            }
        }
        
        // Calcular error
        for(int i = 0; i < n; i++) {
            double diff = fabs(X[i] - X_old[i]);
            if(diff > error) {
                error = diff;
            }
        }
        
        // Mostrar iteración cada 5 pasos o en las primeras 10
        if(iter <= 10 || iter % 5 == 0 || error <= tolerancia) {
            printf("Iteracion %d: ", iter);
            for(int i = 0; i < n; i++) {
                printf("X%d=%.4f ", i+1, X[i]);
            }
            printf(" Error: %.6f\n", error);
        }
    }
    
    // Mostrar resultados finales
    if(iter > max_iter) {
        printf("\nSe alcanzo el maximo de iteraciones (%d)\n", max_iter);
    } else {
        printf("\nConvergencia alcanzada en %d iteraciones\n", iter-1);
    }
    
    printf("Error final: %.8f\n", error);
    
    printf("\nSOLUCION (Minimos Cuadrados):\n");
    for(int i = 0; i < n; i++) {
        printf("x%d = %12.8f\n", i+1, X[i]);
    }
    
    // Verificar solución de mínimos cuadrados
    printf("\nVERIFICACION (Minimos Cuadrados):\n");
    double error_cuadratico = 0.0;
    for(int i = 0; i < m; i++) {
        double suma = 0.0;
        for(int j = 0; j < n; j++) {
            suma += A[i][j] * X[j];
        }
        double residuo = suma - B[i];
        error_cuadratico += residuo * residuo;
        printf("Ecuacion %d: %.6f = %.6f (Residuo: %.6f)\n", 
               i+1, suma, B[i], residuo);
    }
    printf("Error cuadratico total: %.8f\n", error_cuadratico);
    
    // Verificar convergencia del sistema normal
    printf("\nANALISIS DE CONVERGENCIA:\n");
    
    int dominante = 1;
    for(int i = 0; i < n; i++) {
        double suma = 0.0;
        for(int j = 0; j < n; j++) {
            if(j != i) {
                suma += fabs(ATA[i][j]);
            }
        }
        if(fabs(ATA[i][i]) <= suma) {
            dominante = 0;
            printf("Fila %d: |%.2f| <= %.2f (No dominante)\n", 
                   i+1, fabs(ATA[i][i]), suma);
        } else {
            printf("Fila %d: |%.2f| > %.2f (Dominante)\n", 
                   i+1, fabs(ATA[i][i]), suma);
        }
    }
    
    if(dominante) {
        printf("La matriz A^T*A es diagonalmente dominante - Convergencia garantizada\n");
    } else {
        printf("La matriz A^T*A NO es diagonalmente dominante - Convergencia no garantizada\n");
    }
    
    // Analizar tipo de sistema
    printf("\nANALISIS DEL SISTEMA ORIGINAL:\n");
    printf("Ecuaciones: %d, Incognitas: %d\n", m, n);
    if(m > n) {
        printf("Sistema SOBREDETERMINADO (mas ecuaciones que incognitas)\n");
        printf("Solucion de MINIMOS CUADRADOS\n");
    } else if(m < n) {
        printf("Sistema SUBDETERMINADO (mas incognitas que ecuaciones)\n");
        printf("Infinitas soluciones - Se muestra una solucion particular\n");
    } else {
        printf("Sistema DETERMINADO (igual numero de ecuaciones e incognitas)\n");
    }
    
    // Liberar memoria
    for(int i = 0; i < m; i++) {
        free(A[i]);
    }
    for(int i = 0; i < n; i++) {
        free(ATA[i]);
    }
    free(A);
    free(ATA);
    free(B);
    free(ATB);
    free(X);
    free(X_old);
    
    return 0;
}