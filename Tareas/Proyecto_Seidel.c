/*
 * ============================================================================
 * SISTEMA DE FERMENTACIÓN DE CERVEZA ARTESANAL - GAUSS-SEIDEL
 * ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 10
#define MAX_ITER 500
#define TOLERANCIA 1e-6 // Tolerancia para la convergencia

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
void validarResultadosFisicos(double *sol, Variable vars[N]); // Ya corregida la sintaxis

// Implementación del método iterativo
int gaussSeidel(double **A, double *B, int n, double *X);

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
    // x3* (CO2/1000) y x4* (Levadura/10) para un mejor condicionamiento y dominancia diagonal.
    
    // Matriz A (Ajustada para favorecer la dominancia diagonal necesaria en Gauss-Seidel)
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
    
    // Vector B (ajustado para la convergencia y solución deseada)
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
        // Se imprime la solución real (re-escalada) a 4 decimales para la tabla de validación
        printf("| %-15s | %10.4f | %10.2f | %10.2f | %-10s |\n",
               vars[i].nombre, X_original[i], vars[i].valor_min, vars[i].valor_max, estado);
    }
    
    if (fuera_rango == 0) {
        printf("\n Éxito: Todos los parámetros están dentro de los rangos físico-químicos aceptables.\n");
    } else {
        printf("\n Advertencia: %d parámetros fuera de rango. El sistema de ecuaciones requiere ajuste.\n", fuera_rango);
    }
}

// ============================================================================
// IMPLEMENTACIÓN: MÉTODO GAUSS-SEIDEL
// ============================================================================

int gaussSeidel(double **A, double *B, int n, double *X) {
    // Inicializar el vector solución con una estimación inicial
    for (int i = 0; i < n; i++) {
        X[i] = 0.0;
    }

    double *X_old = (double *)malloc(n * sizeof(double));
    if (!X_old) {
        printf("ERROR: Fallo de asignación de memoria.\n");
        return -1;
    }
    
    int iteraciones = 0;
    printf("\n[Ejecutando Gauss-Seidel...]\n");

    for (iteraciones = 0; iteraciones < MAX_ITER; iteraciones++) {
        // Guardar la solución previa
        for (int i = 0; i < n; i++) {
            X_old[i] = X[i];
        }

        double error_max = 0.0; // Usaremos la norma infinito para el error

        // Iteración de Gauss-Seidel
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    // Usamos el valor más reciente de X[j] (X[j] actualizado si j < i, X[j] de X_old si j > i)
                    sum += A[i][j] * X[j]; 
                }
            }
            
            if (fabs(A[i][i]) < 1e-12) {
                printf("ERROR: Pivote nulo. El método Gauss-Seidel falló.\n");
                free(X_old);
                return -1;
            }

            double X_new = (B[i] - sum) / A[i][i];
            
            // Actualizar el error
            double error_actual = fabs(X_new - X_old[i]);
            if (error_actual > error_max) {
                error_max = error_actual;
            }

            X[i] = X_new;
        }

        // Condición de parada
        if (error_max < TOLERANCIA) {
            printf("\n Convergencia exitosa en %d iteraciones.\n", iteraciones + 1);
            free(X_old);
            return iteraciones + 1;
        }
    }

    printf("\n Falla de Convergencia: No se alcanzó la tolerancia en %d iteraciones.\n", MAX_ITER);
    free(X_old);
    return MAX_ITER; 
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
    
    printf("=== SISTEMA DE OPTIMIZACIÓN DE FERMENTACIÓN (GAUSS-SEIDEL) ===\n");
    mostrarMatriz(A, B, n);
    
    // Resolver
    int iter = gaussSeidel(A, B, n, X);
    
    // Mostrar resultados solo si hubo éxito
    if (iter > 0) {
        printf("\n=== SOLUCIÓN FINAL (Variables Escaladas X*) - DETALLE DE PRECISIÓN ===\n");
        printf("Muestra el error de truncamiento frente a los métodos directos.\n");
        
        for(int i = 0; i < n; i++) {
            // Se utiliza ALTA PRECISIÓN (%.10f) para que se vean las diferencias decimales
            printf("%-15s: %.10f\n", vars[i].nombre, X[i]); 
        }
        
        // Validación
        validarResultadosFisicos(X, vars); 
    }
    
    // Liberar memoria
    for(int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    free(B);
    free(X);
    
    return 0;
}
