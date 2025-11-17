/*
 * ============================================================================
 * SISTEMA DE FERMENTACIÓN DE CERVEZA ARTESANAL - VERSIÓN FINAL LU
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 10

// ============================================================================
// ESTRUCTURAS Y PROTOTIPOS
// ============================================================================

typedef struct {
    char nombre[20];
    char unidad[20];
    double valor_min;
    double valor_max;
} Variable;

void inicializarVariables(Variable vars[N]);
void setupSistemaCerveza(double **A, double *B);
void mostrarMatriz(double **A, double *B, int n);
void validarResultadosFisicos(double *sol, Variable vars[N]);

int factorizacionLU(double **A, double *B, int n, double *X);

// ============================================================================
// IMPLEMENTACIÓN: UTILIDADES Y CONFIGURACIÓN
// ============================================================================

void inicializarVariables(Variable vars[N]) {
    // Definiciones clave para la validación (RANGOS FÍSICOS)
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
    // Matriz A (NO CAMBIA, representa las relaciones físico-químicas)
    double filas[10][10] = {
        /* x0 Az. | x1 Ext. | x2 Alc. | x3* CO2 | x4* Lev. | x5 Sab. | x6 Mad. | x7 Clar. | x8 Carb. | x9 Est. |*/
        { 1.0,  -0.05,   0.0,     0.0,      0.0,      0.0,     0.0,     0.0,      0.0,       0.0}, 
        { -0.4,    1.0,   0.0,     0.0,      0.0,      0.0,     0.0,     0.0,      0.0,       0.0},
        { -0.48,   0.0,   1.0,     0.0,      0.1,      0.0,     0.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   0.0,     1.0,     -0.0005,   0.0,     0.0,     0.0,      0.0,       0.0},
        { -0.22,   0.0,   0.07,    0.0001,   1.0,      0.0,     0.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   -5.5,    0.012,    0.0,      1.0,    -0.8,     0.0,      0.0,       0.0},
        { 0.0,     0.0,  0.12,     0.0,      0.0,     -0.025,   1.0,     0.0,      0.0,       0.0},
        { 0.0,     0.0,   0.0,    -0.00008, 0.035,     0.0,     0.12,    1.0,      0.0,       0.0},
        { 0.0,     0.0,   0.04,  -0.0008,    0.0,    -0.015,    0.0,     0.0,      1.0,       0.0},
        { 0.0,     0.0,  -0.02,    0.0,      0.0,     0.02,    -0.05,    0.015,    0.0,       1.0}
    };
    
    // Vector B (NUEVO ESCENARIO: Valores ligeramente ajustados para cambiar la solución)
    // Buscamos un poco más de alcohol y menos maduración.
    double B_vals[] = {15.5, 2.7, 0.6, 2.3, 4.2, 5.2, 1.6, 9.8, 0.9, 4.1};

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
    printf("\n\n=== VALIDACIÓN FÍSICA DEL PROCESO (NUEVO ESCENARIO) ===\n");
    printf("| %-15s | %-10s | %-10s | %-10s | %-10s |\n", "Variable", "Solución", "Mínimo", "Máximo", "Estado");
    printf("|%s|%s|%s|%s|%s|\n", "---------------", "----------", "----------", "----------", "----------");

    int fuera_rango = 0;
    
    // Re-escalar las variables 3 (CO2) y 4 (Levadura) a sus unidades originales
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
        // Imprimimos la solución re-escalada con 4 decimales para la tabla de validación
        printf("| %-15s | %10.4f | %10.2f | %10.2f | %-10s |\n",
               vars[i].nombre, X_original[i], vars[i].valor_min, vars[i].valor_max, estado);
    }
    
    if (fuera_rango == 0) {
        printf("\nExito: Todos los parámetros están dentro de los rangos físico-químicos aceptables.\n");
    } else {
        printf("\n️ Advertencia: %d parámetros fuera de rango. El nuevo escenario requiere ajuste fino.\n", fuera_rango);
    }
}

int factorizacionLU(double **A, double *B, int n, double *X) {
    double *Y = (double *)malloc(n * sizeof(double));
    if (!Y) {
        printf("ERROR: Fallo de asignación de memoria para vector intermedio Y.\n");
        return 0;
    }
    
    printf("\n[Ejecutando Factorización LU...]\n");

    // 1. Descomposición LU (Doolittle)
    for (int k = 0; k < n; k++) {
        // Cálculo de U[k][j]
        for (int j = k; j < n; j++) {
            double sum = 0.0;
            for (int s = 0; s < k; s++) {
                sum += A[k][s] * A[s][j]; 
            }
            A[k][j] = A[k][j] - sum; 
        }
        
        // Cálculo de L[i][k]
        for (int i = k + 1; i < n; i++) {
            double sum = 0.0;
            for (int s = 0; s < k; s++) {
                sum += A[i][s] * A[s][k];
            }
            double pivote = A[k][k];
            if (fabs(pivote) < 1e-12) {
                 printf("ERROR: Pivote nulo. El sistema es singular.\n");
                 free(Y);
                 return 0;
            }
            A[i][k] = (A[i][k] - sum) / pivote;
        }
    }
    
    // 2. Sustitución hacia adelante: L * Y = B
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += A[i][j] * Y[j]; 
        }
        Y[i] = B[i] - sum; 
    }
    
    // 3. Sustitución hacia atrás: U * X = Y
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += A[i][j] * X[j]; 
        }
        double pivote = A[i][i];
        if (fabs(pivote) < 1e-12) {
             printf("ERROR: Pivote nulo en U. Factorización fallida.\n");
             free(Y);
             return 0;
        }
        X[i] = (Y[i] - sum) / pivote;
    }
    
    free(Y);
    printf(" Solución obtenida mediante método directo LU.\n");
    return 1;
}

// ============================================================================
// FUNCIÓN PRINCIPAL
// ============================================================================

int main() {
    int n = N;
    
    // Reservar memoria
    double **A_original = malloc(n * sizeof(double *));
    double *B = malloc(n * sizeof(double));
    double *X = malloc(n * sizeof(double));
    Variable vars[N];
    
    for(int i = 0; i < n; i++) {
        A_original[i] = malloc(n * sizeof(double));
    }
    
    // Inicializar y configurar con el NUEVO ESCENARIO
    inicializarVariables(vars);
    setupSistemaCerveza(A_original, B);
    
    printf("=== SISTEMA DE OPTIMIZACIÓN DE FERMENTACIÓN (LU) - ESCENARIO B ===\n");
    mostrarMatriz(A_original, B, n);
    
    // Resolver
    int success = factorizacionLU(A_original, B, n, X);
    
    // Mostrar resultados solo si hubo éxito
    if (success) {
        printf("\n=== SOLUCIÓN FINAL (Variables Escaladas X*) - DETALLE DE PRECISIÓN ===\n");
        printf("Estos resultados son únicos para este Escenario B.\n");
        
        for(int i = 0; i < n; i++) {
            // Alta precisión para comparar con el resultado de Gauss-Seidel
            printf("%-15s: %.10f\n", vars[i].nombre, X[i]); 
        }
        
        // Validación
        validarResultadosFisicos(X, vars);
    }
    
    // Liberar memoria
    for(int i = 0; i < n; i++) {
        free(A_original[i]);
    }
    free(A_original);
    free(B);
    free(X);
    
    return 0;
}