#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Función para leer desde archivo
int leerDesdeArchivo(const char *filename, double ***A, double **B, int *m, int *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return 0;
    }
    
    fscanf(file, "%d %d", m, n);
    
    // Asignar memoria
    *A = (double **)malloc(*m * sizeof(double *));
    *B = (double *)malloc(*m * sizeof(double));
    
    for(int i = 0; i < *m; i++) {
        (*A)[i] = (double *)malloc(*n * sizeof(double));
        for(int j = 0; j < *n; j++) {
            fscanf(file, "%lf", &(*A)[i][j]);
        }
    }
    
    for(int i = 0; i < *m; i++) {
        fscanf(file, "%lf", &(*B)[i]);
    }
    
    fclose(file);
    return 1;
}

// Función para guardar matrices L y U
void guardarMatricesLU(const char *filename, double **L, double **U, int m, int n) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error: No se pudo crear el archivo %s\n", filename);
        return;
    }
    
    fprintf(file, "%d %d\n", m, n);
    
    // Guardar L
    fprintf(file, "L\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            fprintf(file, "%.6f ", L[i][j]);
        }
        fprintf(file, "\n");
    }
    
    // Guardar U
    fprintf(file, "U\n");
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            fprintf(file, "%.6f ", U[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
    printf("Matrices L y U guardadas en %s\n", filename);
}

// Función para calcular determinante (solo para matrices cuadradas)
double calcularDeterminante(double **U, int n) {
    double det = 1.0;
    for(int i = 0; i < n; i++) {
        det *= U[i][i];
    }
    return det;
}

// Función para normalizar ecuaciones
void normalizarSistema(double **A, double *B, int m, int n) {
    printf("Normalizando sistema...\n");
    for(int i = 0; i < m; i++) {
        // Encontrar el máximo valor absoluto en la fila
        double max_val = 0.0;
        for(int j = 0; j < n; j++) {
            if(fabs(A[i][j]) > max_val) {
                max_val = fabs(A[i][j]);
            }
        }
        
        // Normalizar si el máximo no es cero
        if(max_val > 1e-10) {
            printf("Fila %d: factor de normalizacion = %.4f\n", i+1, max_val);
            for(int j = 0; j < n; j++) {
                A[i][j] /= max_val;
            }
            B[i] /= max_val;
        } else {
            printf("Fila %d: todos los coeficientes son cero\n", i+1);
        }
    }
}

// Función para inicializar matrices
void inicializarMatrices(double ***A, double ***L, double ***U, double **B, double **Y, double **X, int **pivote, int m, int n) {
    *A = (double **)malloc(m * sizeof(double *));
    *L = (double **)malloc(m * sizeof(double *));
    *U = (double **)malloc(m * sizeof(double *));
    *B = (double *)malloc(m * sizeof(double));
    *Y = (double *)malloc(m * sizeof(double));
    *X = (double *)malloc(n * sizeof(double));
    *pivote = (int *)malloc(m * sizeof(int));
    
    for(int i = 0; i < m; i++) {
        (*A)[i] = (double *)malloc(n * sizeof(double));
        (*L)[i] = (double *)malloc(m * sizeof(double));
        (*U)[i] = (double *)malloc(n * sizeof(double));
        (*pivote)[i] = i;
    }
    
    // Inicializar L y U
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < m; j++) {
            (*L)[i][j] = 0.0;
        }
        for(int j = 0; j < n; j++) {
            (*U)[i][j] = 0.0;
        }
    }
}

// Función para liberar memoria
void liberarMemoria(double **A, double **L, double **U, double *B, double *Y, double *X, int *pivote, int m) {
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
}

int main() {
    int m, n;
    double **A, **L, **U, *B, *Y, *X;
    int *pivote;
    
    printf("RESOLUCION DE SISTEMAS DE ECUACIONES\n");
    printf("Metodo de Factorizacion LU para Matrices Rectangulares\n\n");
    
    // MENU PARA SELECCIONAR ENTRADA
    printf("Seleccione opcion de entrada:\n");
    printf("1. Ingresar datos por terminal\n");
    printf("2. Leer datos desde archivo\n");
    printf("Opcion: ");
    
    int opcion;
    scanf("%d", &opcion);
    
    if(opcion == 2) {
        char filename[100];
        printf("Nombre del archivo: ");
        scanf("%s", filename);
        
        if(!leerDesdeArchivo(filename, &A, &B, &m, &n)) {
            printf("Usando entrada por terminal...\n");
            opcion = 1; // Forzar entrada por terminal
        }
    }
    
    if(opcion == 1) {
        // Entrada por terminal
        printf("Numero de ecuaciones (m): ");
        scanf("%d", &m);
        printf("Numero de incognitas (n): ");
        scanf("%d", &n);
        
        // Inicializar matrices
        inicializarMatrices(&A, &L, &U, &B, &Y, &X, &pivote, m, n);
        
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
    }
    
    // Mostrar sistema original
    printf("\nSISTEMA ORIGINAL (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.4f", A[i][j]);
        }
        printf("  |%8.4f\n", B[i]);
    }
    
    // NORMALIZACIÓN
    printf("\n=== APLICANDO NORMALIZACION ===\n");
    normalizarSistema(A, B, m, n);
    
    // Mostrar sistema normalizado
    printf("\nSISTEMA NORMALIZADO (%dx%d):\n", m, n);
    for(int i = 0; i < m; i++) {
        for(int j = 0; j < n; j++) {
            printf("%8.4f", A[i][j]);
        }
        printf("  |%8.4f\n", B[i]);
    }
    
    // FACTORIZACION LU CON PIVOTEO
    printf("\n=== FACTORIZACION LU ===\n");
    
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
    }
    
    printf("\nRango de la matriz: %d\n", rango);
    
    // CÁLCULO DEL DETERMINANTE (solo para matrices cuadradas)
    if(m == n) {
        double det = calcularDeterminante(U, n);
        printf("\n=== ANALISIS DEL DETERMINANTE ===\n");
        printf("Determinante: %.6f\n", det);
        
        if(fabs(det) < 1e-10) {
            printf("ADVERTENCIA: El determinante es cercano a cero\n");
            printf("El sistema puede estar mal condicionado o ser singular\n");
        } else {
            printf("El sistema esta bien determinado\n");
        }
    } else {
        printf("\n=== ANALISIS DEL SISTEMA ===\n");
        printf("Sistema rectangular (%dx%d)\n", m, n);
        printf("Rango: %d\n", rango);
    }
    
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
    
    // GUARDAR MATRICES L Y U
    printf("\n¿Desea guardar las matrices L y U? (1=Si, 0=No): ");
    int guardar;
    scanf("%d", &guardar);
    if(guardar) {
        char filename[100];
        printf("Nombre del archivo para guardar: ");
        scanf("%s", filename);
        guardarMatricesLU(filename, L, U, m, n);
    }
    
    // RESOLVER LY = B
    printf("\n=== RESOLVIENDO LY = B ===\n");
    for(int i = 0; i < m; i++) {
        Y[i] = B[i];
        for(int j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
        Y[i] = Y[i] / L[i][i];
        printf("y%d = %.4f\n", i+1, Y[i]);
    }
    
    // RESOLVER UX = Y
    printf("\n=== RESOLVIENDO UX = Y ===\n");
    
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
    printf("\n=== SOLUCION ===\n");
    for(int i = 0; i < n; i++) {
        printf("x%d = %8.4f\n", i+1, X[i]);
    }
    
    // Verificar solución
    printf("\n=== VERIFICACION ===\n");
    for(int i = 0; i < m; i++) {
        double suma = 0;
        for(int j = 0; j < n; j++) {
            suma += A[i][j] * X[j];
        }
        printf("Ecuacion %d: %.4f = %.4f", i+1, suma, B[i]);
        if(fabs(suma - B[i]) < 1e-5) {
            printf("  ✓\n");
        } else {
            printf("  ✗ (Error: %.4f)\n", fabs(suma - B[i]));
        }
    }
    
    // Analizar tipo de solución
    printf("\n=== ANALISIS FINAL ===\n");
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
    liberarMemoria(A, L, U, B, Y, X, pivote, m);
    
    return 0;
}
