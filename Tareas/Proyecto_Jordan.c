/*
 * ============================================================================
 * SISTEMA DE FERMENTACIÓN DE CERVEZA ARTESANAL - VERSIÓN GAUSS-JORDAN
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 10

// Definición de estructura (simplificada) para Validación Física
typedef struct {
    char nombre[20];
    char unidad[20];
    double valor_min;
    double valor_max;
} Variable;

// ============================================================================
// IMPLEMENTACIÓN: UTILIDADES
// ============================================================================

void inicializarVariables(Variable vars[N]) {
    double min_vals[] = {10.0, 10.0, 4.5, 1800.0, 40.0, 30.0, 10.0, 5.0, 2.2, 8.0};
    double max_vals[] = {25.0, 18.0, 8.0, 2500.0, 100.0, 70.0, 30.0, 50.0, 2.8, 20.0};
    char *nombres[] = {"Azucares", "Extracto", "Alcohol", "CO2", "Levadura",
                       "Sabores", "Maduracion", "Claridad", "Carbonatacion", "Estabilidad"};
    char *unidades[] = {"g/L", "°P", "% vol", "mg/L", "M cel/mL",
                         "UBA", "dias", "NTU", "vol", "semanas"};

    for(int i = 0; i < N; i++) {
        strcpy(vars[i].nombre, nombres[i]);
        strcpy(vars[i].unidad, unidades[i]);
        vars[i].valor_min = min_vals[i];
        vars[i].valor_max = max_vals[i];
    }
}

void setupSistemaCerveza(double **A, double *B) {
    // Matriz A (Relaciones físicas simplificadas y estabilizadas)
    double filas[10][10] = {
        /* x0 Az. | x1 Ext. | x2 Alc. | x3* CO2 | x4* Lev. | x5 Sab. | x6 Mad. | x7 Clar. | x8 Carb. | x9 Est. |*/
        { 1.0,  -0.05,   0.0,     0.0,      0.0,      0.0,     0.0,     0.0,      0.0,       0.0},
        { -0.4,    1.0,   0.0,     0.0,      0.0,      0.0,     0.0,     0.0,      0.0,       0.0},
        { -0.5,    0.0,   1.0,     0.0,      0.1,      0.0,     0.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   0.0,     1.0,     -0.0005,   0.0,     0.0,     0.0,      0.0,       0.0},
        { -0.2,    0.0,   0.07,    0.0001,   1.0,      0.0,     0.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   -5.5,    0.012,    0.0,      1.0,    -0.8,     0.0,      0.0,       0.0},
        { 0.0,     0.0,  0.1,      0.0,      0.0,     -0.02,    1.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   0.0,    -0.00005, 0.03,      0.0,     0.1,     1.0,      0.0,       0.0},
        { 0.0,     0.0,   0.05,  -0.001,     0.0,    -0.01,     0.0,     0.0,      1.0,       0.0},
        { 0.0,     0.0,  -0.01,    0.0,      0.0,     0.02,    -0.03,    0.01,     0.0,       1.0}
    };
    
    // Vector B (OPTIMIZADO para soluciones dentro de rango)
    double B_vals[] = {15.0, 2.5, 0.5, 2.2, 4.5, 5.0, 1.8, 10.0, 0.8, 4.0};

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            A[i][j] = filas[i][j];
        }
        B[i] = B_vals[i];
    }
}

void mostrarMatriz(double **A, double *B, int n) {
    printf("\nSistema de Ecuaciones (Matriz A | Vector B):\n");
    for(int i = 0; i < n; i++) {
        printf("  [");
        for(int j = 0; j < n; j++) {
            printf(" %8.4f ", A[i][j]);
        }
        printf("] | %8.4f\n", B[i]);
    }
}

void validarResultadosFisicos(double *sol, Variable vars[N]) {
    printf("\n\n=== VALIDACIÓN FÍSICA DEL PROCESO ===\n");
    printf("| %-15s | %-10s | %-10s | %-10s | %-10s |\n", "Variable", "Solución", "Mínimo", "Máximo", "Estado");
    printf("|%s|%s|%s|%s|%s|\n", "---------------", "----------", "----------", "----------", "----------");

    int fuera_rango = 0;
    
    // Re-escalar las variables 3 y 4 de vuelta a sus unidades originales
    double X_original[N];
    for (int i = 0; i < N; i++) {
        X_original[i] = sol[i];
    }
    X_original[3] *= 1000.0; // CO2: x3* * 1000
    X_original[4] *= 10.0;   // Levadura: x4* * 10
    
    for (int i = 0; i < N; i++) {
        char *estado;
        if (X_original[i] >= vars[i].valor_min && X_original[i] <= vars[i].valor_max) {
            estado = "OK";
        } else {
            estado = "FUERA";
            fuera_rango++;
        }
        printf("| %-15s | %10.4f | %10.2f | %10.2f | %-10s |\n",
               vars[i].nombre, X_original[i], vars[i].valor_min, vars[i].valor_max, estado);
    }
    
    if (fuera_rango == 0) {
        printf("\n Éxito: Todos los parámetros están dentro de los rangos físico-químicos aceptables.\n");
    } else {
        printf("\n️ Advertencia: %d parámetros fuera de rango. El sistema de ecuaciones requiere ajuste.\n", fuera_rango);
    }
}

void gaussJordan(double **A, double *B, int n, double *X) {
    // 1. Fase de Eliminación e Identidad
    for(int i = 0; i < n; i++) {
        // Pivoteo parcial
        int max_fila = i;
        for(int k = i + 1; k < n; k++) {
            if(fabs(A[k][i]) > fabs(A[max_fila][i])) {
                max_fila = k;
            }
        }
        
        // Intercambiar filas (si es necesario)
        if(max_fila != i) {
            for(int j = 0; j < n; j++) {
                double temp = A[i][j];
                A[i][j] = A[max_fila][j];
                A[max_fila][j] = temp;
            }
            double temp_b = B[i];
            B[i] = B[max_fila];
            B[max_fila] = temp_b;
        }
        
        double pivote = A[i][i];
        if(fabs(pivote) < 1e-12) {
             printf("ERROR: Pivote nulo. El sistema es singular.\n");
             return; 
        }
        
        // Normalizar la fila actual
        for(int j = 0; j < n; j++) {
            A[i][j] /= pivote;
        }
        B[i] /= pivote;
        
        // Eliminar los coeficientes no diagonales
        for(int k = 0; k < n; k++) {
            if(k != i) {
                double factor = A[k][i];
                for(int j = 0; j < n; j++) {
                    A[k][j] -= factor * A[i][j];
                }
                B[k] -= factor * B[i];
            }
        }
    }
    
    // 2. Solución
    for(int i = 0; i < n; i++) {
        X[i] = B[i];
    }
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

int main() {
    int n = N;
    
    // Reservar memoria
    double **A = malloc(n * sizeof(double *));
    double *B = malloc(n * sizeof(double));
    double *X = malloc(n * sizeof(double));
    Variable vars[N];
    
    for(int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
    }
    
    // Inicializar y configurar
    inicializarVariables(vars);
    setupSistemaCerveza(A, B);
    
    printf("=== SISTEMA DE OPTIMIZACIÓN DE FERMENTACIÓN (GAUSS-JORDAN) ===\n");
    mostrarMatriz(A, B, n);
    
    // Resolver
    gaussJordan(A, B, n, X);
    
    // Mostrar resultados con ALTA PRECISIÓN para notar la diferencia con Gauss-Seidel
    printf("\n=== SOLUCIÓN DEL SISTEMA (Variables Escaladas X*) - DETALLE DE PRECISIÓN (GAUSS-JORDAN) ===\n");
    printf("Esta solución representa el valor *exacto* dentro de la precisión de 'double'.\n");
    
    for(int i = 0; i < n; i++) {
        // CORRECCIÓN: Imprimir a 10 decimales
        printf("%-15s: %.10f\n", vars[i].nombre, X[i]); 
    }
    
    // Validación
    validarResultadosFisicos(X, vars);
    
    // Liberar memoria
    for(int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    free(X);
    
    return 0;
}