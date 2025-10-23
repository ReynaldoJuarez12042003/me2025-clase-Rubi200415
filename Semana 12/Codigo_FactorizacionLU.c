#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int m, n;
    
    printf("RESOLUCION DE SISTEMAS DE ECUACIONES\n");
    printf("Metodo de Factorizacion LU para Matrices Rectangulares\n\n");
    
    printf("Numero de ecuaciones (m): ");
    scanf("%d", &m);
    printf("Numero de incognitas (n): ");
    scanf("%d", &n);
    
    // Crear matrices
    double **A = (double **)malloc(m * sizeof(double *));
    double **L = (double **)malloc(m * sizeof(double *));
    double **U = (double **)malloc(m * sizeof(double *));
    double *B = (double *)malloc(m * sizeof(double));
    double *Y = (double *)malloc(m * sizeof(double));
    double *X = (double *)malloc(n * sizeof(double));
    int *pivote = (int *)malloc(m * sizeof(int));
    
    for(int i = 0; i < m; i++) {
        A[i] = (double *)malloc(n * sizeof(double));
        L[i] = (double *)malloc(m * sizeof(double));
        U[i] = (double *)malloc(n * sizeof(double));
        pivote[i] = i;  // Inicializar vector de pivoteo
    }
    
    // Inicializar L y U
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            L[i][j] = 0.0;
        }
        for(int j = 0; j < n; j++) {
            U[i][j] = 0.0;
        }
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
    
    // Mostrar sistema
    printf("\nSISTEMA ORIGINAL (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.2f", A[i][j]);
        }
        printf("  |%8.2f\n", B[i]);
    }
    
    // FACTORIZACION LU CON PIVOTEO
    printf("\nFACTORIZACION LU:\n");
    
    // Copiar A a U
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            U[i][j] = A[i][j];
        }
    }
    
    int rango = 0;
    int min_dim = (m < n) ? m : n;
    
    for(int i = 0; i < min_dim; i++) {
        // Pivoteo parcial
        int max_fila = i;
        double max_valor = fabs(U[i][i]);
        
        for(int k = i + 1; k < m; k++) {
            if(fabs(U[k][i]) > max_valor) {
                max_valor = fabs(U[k][i]);
                max_fila = k;
            }
        }
        
        // Intercambiar filas si es necesario
        if(max_fila != i) {
            // Intercambiar en U
            for(int j = 0; j < n; j++) {
                double temp = U[i][j];
                U[i][j] = U[max_fila][j];
                U[max_fila][j] = temp;
            }
            
            // Intercambiar en B
            double temp_b = B[i];
            B[i] = B[max_fila];
            B[max_fila] = temp_b;
            
            // Actualizar pivoteo
            int temp_p = pivote[i];
            pivote[i] = pivote[max_fila];
            pivote[max_fila] = temp_p;
            
            printf("Intercambiadas filas %d y %d\n", i+1, max_fila+1);
        }
        
        // Verificar si el pivote es cero
        if(fabs(U[i][i]) < 1e-10) {
            printf("Pivote cero en columna %d\n", i+1);
            continue;
        }
        
        rango++;
        
        // Matriz L - diagonal = 1
        L[i][i] = 1.0;
        
        // Eliminacion gaussiana
        for(int k = i + 1; k < m; k++) {
            L[k][i] = U[k][i] / U[i][i];
            
            for(int j = i; j < n; j++) {
                U[k][j] -= L[k][i] * U[i][j];
            }
        }
        
        // Mostrar progreso
        printf("\nPaso %d (Pivote: %.3f):\n", i+1, U[i][i]);
        printf("Matriz L:\n");
        for(int k = 0; k < m; k++) {
            for(int j = 0; j < m; j++) {
                printf("%8.3f", L[k][j]);
            }
            printf("\n");
        }
        printf("Matriz U:\n");
        for(int k = 0; k < m; k++) {
            for(int j = 0; j < n; j++) {
                printf("%8.3f", U[k][j]);
            }
            printf("\n");
        }
    }
    
    printf("\nRango de la matriz: %d\n", rango);
    
    // Mostrar matrices finales
    printf("\nMATRICES FINALES:\n");
    printf("Matriz L (%dx%d):\n", m, m);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            printf("%8.3f", L[i][j]);
        }
        printf("\n");
    }
    
    printf("\nMatriz U (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.3f", U[i][j]);
        }
        printf("\n");
    }
    
    // RESOLVER LY = B
    printf("\nRESOLVIENDO LY = B:\n");
    for(int i = 0; i < m; i++) {
        Y[i] = B[i];
        for(int j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
        Y[i] = Y[i] / L[i][i];
        printf("y%d = %.4f\n", i+1, Y[i]);
    }
    
    // RESOLVER UX = Y
    printf("\nRESOLVIENDO UX = Y:\n");
    
    // Inicializar solución
    for(int i = 0; i < n; i++) {
        X[i] = 0.0;
    }
    
    // Sustitución hacia atrás
    for(int i = rango - 1; i >= 0; i--) {
        // Encontrar el pivote en la fila i
        int col_pivote = -1;
        for(int j = 0; j < n; j++) {
            if(fabs(U[i][j]) > 1e-10) {
                col_pivote = j;
                break;
            }
        }
        
        if(col_pivote != -1) {
            X[col_pivote] = Y[i];
            for(int j = col_pivote + 1; j < n; j++) {
                X[col_pivote] -= U[i][j] * X[j];
            }
            X[col_pivote] = X[col_pivote] / U[i][col_pivote];
            printf("x%d = %.4f\n", col_pivote+1, X[col_pivote]);
        }
    }
    
    // Mostrar solucion
    printf("\nSOLUCION:\n");
    for(int i = 0; i < n; i++) {
        printf("x%d = %8.4f\n", i+1, X[i]);
    }
    
    // Verificar
    printf("\nVERIFICACION:\n");
    for(int i = 0; i < m; i++) {
        double suma = 0;
        for(int j = 0; j < n; j++) {
            suma += A[i][j] * X[j];
        }
        printf("Ecuacion %d: %.4f = %.4f\n", i+1, suma, B[i]);
    }
    
    // Analizar tipo de solución
    printf("\nANALISIS DEL SISTEMA:\n");
    if(rango < m) {
        printf("- Sistema puede ser incompatible\n");
    }
    if(rango < n) {
        printf("- Sistema tiene infinitas soluciones\n");
        printf("- Grados de libertad: %d\n", n - rango);
    }
    if(rango == n && rango == m) {
        printf("- Sistema tiene solucion unica\n");
    }
    
    // Liberar memoria
    for(int i = 0; i < m; i++) {
        free(A[i]);
        free(L[i]);
        free(U[i]);
    }
    free(A);
    free(L);
    free(U);
    free(B);
    free(Y);
    free(X);
    free(pivote);
    
    return 0;
}